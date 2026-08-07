# Конфигурация

> **wrtstat:** секция `key_cache` наследует параметры агрегатора wrtstat (`aggregation_step_ts`, `resolution`, `reducer_limit`, …). Их смысл и взаимосвязь описаны в документации wrtstat — [GitLab](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/docs-md/README.md?ref_type=heads) · [GitHub](https://github.com/mambaru/wrtstat/blob/master/docs-md/README.md).

Конфигурация модуля задаётся в JSON-файле демона WFC для компонента **`btp-ag`** (интерфейс `iag`). Структура сериализуется через `ag_config_json` и наследует параметры хранилища `storage_options`.

## Общая структура

```json
{
  "key_cache": { ... },
  "key_db":    { ... },
  "data_db":   { ... },
  "btp_trace": true,
  "mtload": false,
  "disable_merge": false
}
```

## Параметры верхнего уровня (`ag_config`)

| Поле | Тип | По умолчанию | Описание |
|------|-----|--------------|----------|
| `key_cache` | object | — | Настройки in-memory кэша и агрегатора |
| `key_db` | object | — | RocksDB для метаданных ключей |
| `data_db` | object | — | Шардированный RocksDB для временных рядов |
| `btp_trace` | bool | `true` | Подробное логирование операций хранилища (канал `BTP_AG`) |
| `mtload` | bool | `false` | Параллельная загрузка БД при нескольких инстансах домена |
| `disable_merge` | bool | `false` | Отключить RocksDB merge; использовать только `Put` вместо `Merge` |

### `btp_trace`

При `true` трассировочные сообщения из `storage` пишутся в лог WFC с уровнем `MESSAGE`. При `false` трассировка отключена.

### `mtload`

Если демон сконфигурирован с несколькими инстансами `btp-ag`, при `mtload: true` каждый инстанс открывает своё хранилище в отдельном потоке. Главный инстанс ждёт завершения загрузки всех остальных перед продолжением старта.

### `disable_merge`

Включать только на время миграции или отладки. При `true` инкрементальные записи в `data_storage` выполняются через `Put` (полная замена), а не через `Merge`.

---

## `key_cache` — кэш ключей и агрегатор

Наследует параметры агрегатора wrtstat (`aggregator_options` → `separator_options` → `reducer_options`).

### Параметры кэша

| Поле | Тип | По умолчанию | Описание |
|------|-----|--------------|----------|
| `TTL` | number | `0` | Время жизни ключей в кэше (секунды). `0` — без ограничения |
| `gc_interval` | number | `0` | Интервал сборки мусора устаревших ключей (секунды). `0` — GC по таймеру отключён |
| `mutex_hash` | number | `1024` | Количество мьютексов для шардирования блокировок в кэше |
| `aggregate_last_point` | bool | `false` | Принудительная агрегация последней незавершённой точки |

### Параметры агрегатора (`aggregator_options`)

| Поле | Тип | По умолчанию | Описание |
|------|-----|--------------|----------|
| `outgoing_reduced_size` | number | `128` | Размер буфера reduced-данных при агрегации |

### Параметры разделения по времени (`separator_options`)

| Поле | Тип | По умолчанию | Описание |
|------|-----|--------------|----------|
| `aggregation_step_ts` | number | `1` | Шаг агрегации в единицах `resolution`. Например, для шага 5 секунд при `resolution: "seconds"` укажите `5` |
| `soiled_start_ts` | number | `0` | Смещение начала агрегации для предотвращения «лавины» записей. Обычно равно `aggregation_step_ts` |
| `resolution` | string | `"seconds"` | Единица временной метки: `"none"`, `"seconds"`, `"milliseconds"`, `"microseconds"`, `"nanoseconds"` |

При `resolution: "none"` временная метка должна передаваться явно в каждом `push`; нулевое значение считается валидным.

Значения `resolution` соответствуют множителям:

| Значение | Множитель |
|----------|-----------|
| `none` | 0 |
| `seconds` | 1 |
| `milliseconds` | 1000 |
| `microseconds` | 1000000 |
| `nanoseconds` | 1000000000 |

### Параметры редуктора (`reducer_options`)

| Поле | Тип | По умолчанию | Описание |
|------|-----|--------------|----------|
| `reducer_limit` | number | `4096` | Лимит элементов в редукторе |
| `reducer_levels` | number | `256` | Количество уровней редуктора |

---

## `key_db` — хранилище метаданных ключей

Одна RocksDB-база (`DBWithTTL`). Ключ — строковое имя метрики, значение — бинарная структура `stored_key`.

| Поле | Тип | По умолчанию | Описание |
|------|-----|--------------|----------|
| `db_path` | string | `"./key_storage"` | Путь к каталогу базы данных |
| `wal_path` | string | `""` | Путь для WAL. Пустая строка — WAL в каталоге БД (поведение RocksDB по умолчанию) |
| `ini_path` | string | `"./key_storage.ini"` | Путь к INI-файлу опций RocksDB |
| `TTL` | number | `0` | TTL записей в секундах (`DBWithTTL`). `0` — без автоматического удаления |
| `create_if_missing` | bool | `true` | Создать базу, если не существует |
| `auto_repair` | bool | `true` | Автоматическое восстановление при повреждении |

При старте домен преобразует `db_path` и `ini_path` в абсолютные пути относительно файла конфигурации демона и создаёт каталоги.

---

## `data_db` — хранилище временных рядов

Шардированная RocksDB (см. [Шардирование RocksDB](rocksdb-sharding.md)).

| Поле | Тип | По умолчанию | Описание |
|------|-----|--------------|----------|
| `hash_size` | number | `128` | Количество шардов (отдельных RocksDB-инстансов). `0` трактуется как `1` |
| `db_path` | string | `"./data_storage"` | Корневой каталог; внутри создаются подкаталоги `0/`, `1/`, …, `hash_size-1/` |
| `wal_path` | string | `""` | Путь для WAL |
| `ini_path` | string | `"./data_storage.ini"` | INI-файл опций RocksDB |
| `TTL` | number | `0` | TTL записей в секундах |
| `result_limit` | number | `3000` | Максимальное количество точек, возвращаемых одним запросом `get` |
| `create_if_missing` | bool | `true` | Создать базу, если не существует |

---

## RocksDB INI-файлы

Параметры `key_db.ini_path` и `data_db.ini_path` указывают на файлы в стандартном формате RocksDB Options File (INI).

Примеры находятся в каталоге `tests/`:

- `tests/key_storage.ini`
- `tests/data_storage.ini`

Файл содержит секции:

| Секция | Описание |
|--------|----------|
| `[Version]` | Версия RocksDB и формата файла |
| `[DBOptions]` | Общие параметры базы (WAL, compaction, лимиты файлов) |
| `[CFOptions "default"]` | Параметры column family (memtable, compression, merge operator в ini) |
| `[TableOptions/BlockBasedTable "default"]` | Параметры табличного формата (block size, bloom filter) |

> **Важно:** для `data_db` merge operator и comparator переопределяются программно при открытии (`BtpMergeOperator`, `key_ts_t comparator`), независимо от значения `merge_operator` в INI.

---

## Пример конфигурации

```json
{
  "key_cache": {
    "TTL": 86400,
    "gc_interval": 3600,
    "mutex_hash": 1024,
    "aggregate_last_point": false,
    "aggregation_step_ts": 60,
    "soiled_start_ts": 60,
    "resolution": "seconds",
    "outgoing_reduced_size": 128,
    "reducer_limit": 4096,
    "reducer_levels": 256
  },
  "key_db": {
    "db_path": "./storage/key_storage",
    "ini_path": "./config/key_storage.ini",
    "TTL": 0,
    "create_if_missing": true,
    "auto_repair": true
  },
  "data_db": {
    "hash_size": 128,
    "db_path": "./storage/data_storage",
    "ini_path": "./config/data_storage.ini",
    "TTL": 604800,
    "result_limit": 3000,
    "create_if_missing": true
  },
  "btp_trace": false,
  "mtload": false,
  "disable_merge": false
}
```

---

## Регистрация в конфигурации демона

Компонент регистрируется как multiton с именем **`btp-ag`** и интерфейсом **`iag`**. Модуль, в котором он объявлен, — **`ag-module`**.

Типичная структура конфигурации WFC-демона (упрощённо):

```json
{
  "packages": ["wfc_btp"],
  "components": {
    "btp-ag": {
      "instances": [
        {
          "name": "main",
          "options": { ... ag_config ... }
        }
      ]
    }
  }
}
```

Точный формат зависит от конфигурации конкретного демона (btpd). См. [wfcroot](https://github.com/mambaru/wfcroot) и [документацию btpd](http://gitlab.mamba.ru/cpp/docs/blob/master/btpd/Readme.md).
