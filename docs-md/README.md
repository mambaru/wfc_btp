# wfc_btp — документация

**wfc_btp** — WFC-пакет (модуль) для демона **btpd** — сервера сбора и хранения агрегированной статистики на базе RocksDB.

Версия пакета: **0.9.8**

## Содержание

| Документ | Описание |
|----------|----------|
| [Конфигурация](configuration.md) | Структура JSON-конфигурации модуля `ag-module`, RocksDB `.ini`-файлы |
| [JSON-RPC API](jsonrpc-api.md) | Методы записи, чтения, поиска и обслуживания |
| [Шардирование RocksDB](rocksdb-sharding.md) | Устройство хранилища данных, ключи, merge operator |
| [Ссылки на субмодули](external-links.md) | URL документации зависимостей (GitLab · GitHub) |

## Назначение

Демон btpd принимает метрики от клиентов через JSON-RPC, агрегирует их по настраиваемым временным интервалам (например, 5 секунд, 1 минута, 1 час) и сохраняет на диск. Данные выравниваются по временным точкам в зависимости от конфигурации агрегатора.

Типичные поля метрики: `avg`, `min`, `max`, `count`, `lossy`, перцентили (`perc50`, `perc80`, `perc95`, `perc99`, `perc100`) — все **целочисленные**. Формат этих полей определяется библиотекой **wrtstat** (см. ниже).

## wrtstat

**wrtstat** — библиотека общего назначения для сбора и агрегации числовых метрик: временные окна, lossy-сэмплирование, перцентили. Документация: [GitLab](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/docs-md/README.md?ref_type=heads) · [GitHub](https://github.com/mambaru/wrtstat/blob/master/docs-md/README.md). Подключается как субмодуль WFC:

```
external/wfc/external/wrtstat/
```

wfc_btp использует wrtstat на нескольких уровнях:

| Область | Что берётся из wrtstat |
|---------|------------------------|
| JSON-RPC (запись) | Методы `push`, `multi_push`, `del`; типы `aggregated_data`, `aggregated_info` |
| Агрегация в `key_cache` | `key_aggregator` на базе `wrtstat::aggregator`; параметры `aggregation_step_ts`, `resolution`, `reducer_*` |
| Хранение | `aggregated_info` как значение в RocksDB; merge через `aggregated_info::operator+=` |
| Интерфейс домена | `ibtp` наследует `wfc::istatistics` (контракт wrtstat для push/del) |

На стороне **клиента** (сервис, отправляющий метрики в btpd) wrtstat предоставляет метры (`time_meter`, `value_meter`, …) и локальный агрегатор — point создаётся до постановки задачи в очередь, при уничтожении sample попадает в агрегатор и далее может быть отправлен через `push`.

Документация wrtstat в репозитории ([полный список ссылок](external-links.md#wrtstat)):

- `docs-md/README.md` — [GitLab](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/docs-md/README.md?ref_type=heads) · [GitHub](https://github.com/mambaru/wrtstat/blob/master/docs-md/README.md) — описание библиотеки, метры, агрегатор, lossy
- `Readme.md` — [GitLab](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/Readme.md?ref_type=heads) · [GitHub](https://github.com/mambaru/wrtstat/blob/master/Readme.md) — краткое описание и ссылки
- [Doxygen API](https://mambaru.github.io/wrtstat/index.html) — онлайн-справка по API

## Архитектура

Проект состоит из двух слоёв:

```
┌─────────────────────────────────────────────────────────┐
│  package/  — WFC-модуль (wfc_btp)                       │
│    btp_package → ag_module → domain / service / gateway  │
└──────────────────────────┬──────────────────────────────┘
                           │ использует
┌──────────────────────────▼──────────────────────────────┐
│  libbtp/  — библиотека btpstorage (независима от WFC)    │
│    storage → key_cache + key_storage + data_storage      │
└─────────────────────────────────────────────────────────┘
```

### WFC-модуль (`package/`)

| Компонент | Имя в конфиге | Роль |
|-----------|---------------|------|
| `btp_package` | пакет `wfc_btp` | Точка входа, регистрирует модули |
| `ag_module` | `"ag-module"` | Контейнер компонентов |
| `ag_domain_multiton` | `"btp-ag"` | Бизнес-логика, владелец `storage` |
| `ag_service_multiton` | — | JSON-RPC invoke (локальная обработка) |
| `ag_gateway_multiton` | — | JSON-RPC call (удалённый вызов / прокси) |

Интерфейс домена: `ibtp` (наследует `wfc::istatistics` из wrtstat).

### Библиотека хранения (`libbtp/`)

| Компонент | Роль |
|-----------|------|
| `storage` | Фасад: координирует кэш, метаданные и данные |
| `key_cache` | In-memory кэш счётчиков, агрегация «на лету», GC |
| `key_storage` | RocksDB: имя метрики → метаданные (`stored_key`) |
| `data_storage` | Шардированный RocksDB: временные ряды по `key_id` |
| `key_aggregator` | Агрегация точек по интервалам (wrtstat) |
| `merge_operator` | Custom RocksDB merge operator для инкрементального слияния |

### Поток записи (`push` / `add`)

1. **key_cache** — принимает `aggregated_data`, агрегирует в текущий интервал; решает, нужна ли запись метаданных на диск.
2. **key_storage** — сохраняет `stored_key` (если завершена «последняя точка» интервала).
3. **data_storage** — `set` (полная замена) или `inc` (merge через RocksDB) для временного ряда.

### Поток чтения (`get`)

1. Имя метрики → `key_id` из `key_cache`.
2. `data_storage::get` — итерация по ключам `(key_id, ts)` начиная с заданной временной метки.

### Жизненный цикл модуля

| Фаза | Действия |
|------|----------|
| `initialize()` | Инициализация счётчиков инстансов |
| `start()` | Создание директорий БД, `storage::open()`, восстановление кэша из `key_storage`, запуск таймера GC |
| Работа | Обработка JSON-RPC запросов |
| `stop()` | `storage::close()`: сброс кэша на диск, закрытие RocksDB |

При `close()` все данные из in-memory кэша принудительно сбрасываются на диск.

## Сборка

```bash
git clone https://github.com/mambaru/wfc_btp.git
cd wfc_btp
BOOST_ROOT=/path/to/boost make tests
```

CMake-цели:

- `libbtp` → библиотека `btpstorage`
- `package` → библиотека `wfc_btp` (линкуется с `wfc` + `btpstorage`)

## Подключение к демону

Пакет подключается к исполняемому демону через [wfcroot](https://github.com/mambaru/wfcroot). В конфигурации демона указывается пакет `wfc_btp` и один или несколько инстансов компонента `btp-ag`.

Подробнее о btpd: [документация btpd](http://gitlab.mamba.ru/cpp/docs/blob/master/btpd/Readme.md).

## Зависимости

| Библиотека | Назначение |
|------------|------------|
| [wfc](https://github.com/mambaru/wfc) | Фреймворк демона (модули, JSON-RPC, workflow) |
| [wrtstat](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/docs-md/README.md?ref_type=heads) ([GitHub](https://github.com/mambaru/wrtstat/blob/master/docs-md/README.md)) | Протокол и типы статистики (`push`, `aggregated_data`, агрегатор, метры) |
| RocksDB | Персистентное хранилище |
| Boost | filesystem и др. |
| wjson, wlog, wflow, iow, wjrpc | JSON, логи, потоки, сеть, RPC |

## Дополнительные ресурсы

- [Doxygen API](https://mambaru.github.io/wfc_btp/index.html)
- [Coverage report](https://mambaru.github.io/wfc_btp/cov-report/index.html)
- [Репозиторий на GitHub](https://github.com/mambaru/wfc_btp)
