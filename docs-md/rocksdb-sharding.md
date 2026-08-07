# Шардирование RocksDB

Хранилище временных рядов (`data_storage`) использует горизонтальное шардирование: несколько независимых экземпляров RocksDB, между которыми распределяются записи по `key_id`.

Метаданные ключей (`key_storage`) **не шардируются** — это одна RocksDB-база с ключом-строкой (имя метрики).

---

## Общая схема

```
storage
├── key_cache          (in-memory, все key_id)
├── key_storage        (RocksDB, 1 инстанс)
│     key: string name  →  value: stored_key (binary)
│
└── data_storage       (shard_rocksdb)
      ├── shard 0/     (RocksDB DBWithTTL)
      ├── shard 1/
      ├── ...
      └── shard N-1/
            key: key_ts_t (key_id, ts)  →  value: aggregated_info (binary)
```

Количество шардов задаётся параметром `data_db.hash_size` (по умолчанию **128**). При `hash_size: 0` создаётся один шард.

---

## Маршрутизация по шардам

Выбор шарда — простой модульный хеш:

```
shard_index = key_id % hash_size
```

Реализация в `shard_rocksdb::get_db`:

```cpp
return _dbs[key_id % _dbs.size()];
```

`key_id` назначается monotonically в `key_cache` при создании новой метрики и сохраняется в `stored_key`. Один и тот же `key_id` всегда попадает в один и тот же шард.

### Структура каталогов

При открытии создаётся корневой каталог `data_db.db_path` и подкаталоги для каждого шарда:

```
data_storage/
├── 0/
├── 1/
├── 2/
...
└── 127/
```

Каждый подкаталог — полноценная RocksDB-база со своим WAL, SST-файлами и манифестом.

---

## Формат ключей и значений

### key_storage (метаданные)

| | |
|---|---|
| **Ключ** | `std::string` — имя метрики |
| **Значение** | Бинарная структура `stored_key` (фиксированный размер) |

`stored_key` содержит:

- `id` — числовой идентификатор (`key_id_t`, uint64)
- `last_update` — время последнего обновления
- Агрегированные поля: `count`, `lossy`, `avg`, perc*, `min`, `max` (каждое как `stored_pair`: value + ts)

### data_storage (временные ряды)

| | |
|---|---|
| **Ключ** | `key_ts_t` = `std::pair<key_id_t, time_type>` — 16 байт (два uint64) |
| **Значение** | Бинарная структура `aggregated_info` (фиксированный размер) |

`aggregated_info` включает: `ts`, `count`, `lossy`, `avg`, `min`, `max`, `perc50`, `perc80`, `perc95`, `perc99`, `perc100`.

### Comparator

Для column family data_storage используется custom comparator `data_comparator`, который сравнивает ключи как пары `(key_id, ts)`:

1. Сначала по `key_id`
2. При равных `key_id` — по `ts`

Это обеспечивает локальность: все точки одной метрики расположены последовательно и доступны через `Seek` + `Next`.

---

## Операции записи

### `set` — полная замена

```
DB::Put(key_ts_t{key_id, ts}, aggregated_info)
```

Используется когда:

- Завершён интервал агрегации (`is_last == true` в `storage::add`)
- Выполняется `merge` (импорт готовых данных)
- При `disable_merge: true` для всех инкрементальных записей

### `inc` — инкрементальное слияние

```
DB::Merge(key_ts_t{key_id, ts}, aggregated_info)
```

Используется для промежуточных записей в текущем интервале (когда `disable_merge: false`).

---

## Merge Operator

RocksDB merge operator `BtpMergeOperator` (`merge_operator.cpp`) реализует ассоциативное слияние значений `aggregated_info`:

1. Десериализует существующее значение (если есть)
2. Десериализует операнд (новую дельту)
3. Складывает через `aggregated_info::operator+=`
4. Сериализует результат

Оператор `+=` корректно объединяет:

- `avg` — взвешенное среднее по `count`
- `count`, `lossy` — суммирование
- `min` / `max` — min/max
- Перцентили — алгоритм слияния из wrtstat ([GitLab](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/docs-md/README.md?ref_type=heads) · [GitHub](https://github.com/mambaru/wrtstat/blob/master/docs-md/README.md)) — `aggregated_info::operator+=`

При повреждённых данных операнд трактуется как нулевой (оператор всегда возвращает `true`).

> Merge operator и comparator устанавливаются **программно** при открытии шарда, поверх опций из INI-файла.

---

## Операции чтения

### `get(key_id, ts, offset, limit)`

1. Выбирается шард: `key_id % hash_size`
2. Формируется ключ `key_ts_t(key_id, ts)`
3. Iterator `Seek(key)` → последовательный обход `Next()`
4. Остановка когда `key_id` в ключе не совпадает с запрошенным
5. Применяются `offset` и `limit`
6. Если результат превышает `result_limit` — обрезается до последних N точек

При `ts == 0` в `storage::get` вычисляется:

```
ts = (time(nullptr) - TTL) * resolution
```

---

## Операции удаления

### `del(key_id)`

1. Выбирается шард по `key_id`
2. `Seek` к `(key_id, 0)`
3. Последовательное `Delete` всех ключей с данным `key_id` до смены `key_id`

---

## TTL

Обе базы (`key_storage` и каждый шард `data_storage`) открываются как `rocksdb::DBWithTTL`:

- `key_db.TTL` — TTL метаданных ключей
- `data_db.TTL` — TTL точек временных рядов

TTL задаётся в секундах. Значение `0` отключает автоматическое удаление по времени.

---

## Компакция

### `compact` на уровне storage

Вызывает `compact` последовательно для:

1. `key_storage` — один `CompactRange` на всю базу
2. Каждого шарда `data_storage` — `CompactRange` на соответствующий инстанс

Компакция может быть запущена:

- Через JSON-RPC метод `compact`
- Явно при обслуживании

---

## Восстановление при старте

При `storage::open`:

1. Открываются `key_storage` и все шарды `data_storage`
2. `key_storage::load` читает все ключи пакетами по 10000
3. Для каждого `stored_name` вызывается `key_cache::init` — восстановление in-memory состояния
4. `key_cache::gc()` — удаление устаревших ключей из кэша

In-memory кэш **не** загружает временные ряды из `data_storage` — они читаются по запросу.

---

## Закрытие и персистентность

При `storage::close`:

1. `key_cache::release` — сброс всех изменённых ключей из памяти
2. Для каждой пары `(stored_name, aggregated_info[])`:
   - `key_storage::set` — сохранение метаданных
   - `data_storage::set` — сохранение точек
3. Закрытие `key_storage` и всех шардов

Это гарантирует, что данные, агрегированные в памяти, но ещё не записанные на диск, будут сохранены при штатной остановке демона.

---

## Производительность и масштабирование

| Аспект | Поведение |
|--------|-----------|
| **Параллелизм записи** | Разные `key_id` могут писать в разные шарды параллельно |
| **Hot key** | Одна метрика всегда в одном шарде — горячие ключи не распределяются |
| **Количество шардов** | Больше шардов → меньше база на шард, но больше файловых дескрипторов и overhead при компакции |
| **Блокировки** | `key_cache` использует `mutex_hash` мьютексов для снижения contention |

Рекомендации:

- `hash_size: 128` — разумное значение по умолчанию для средних нагрузок
- Увеличивайте `hash_size` при большом количестве метрик и высокой write-нагрузке
- Настраивайте RocksDB INI отдельно для `key_storage` (много мелких ключей) и `data_storage` (append-only временные ряды с merge)

---

## Диаграмма потока данных

```
                    push / multi_push
                           │
                           ▼
                    ┌─────────────┐
                    │  key_cache  │  агрегация, назначение key_id
                    └──────┬──────┘
                           │
              ┌────────────┼────────────┐
              │            │            │
              ▼            ▼            ▼
        key_storage   data_storage   data_storage
        (1 RocksDB)   shard[i]       shard[j]
              │            │            │
              │     key_id % N = i     key_id % N = j
              │            │            │
              ▼            ▼            ▼
         name→meta    (id,ts)→point  (id,ts)→point
```
