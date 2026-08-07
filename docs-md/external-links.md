# Внешние ссылки на субмодули

Документация wfc_btp ссылается на репозитории зависимостей напрямую — не через относительные пути в дереве `external/…`. Так ссылки открываются в веб-интерфейсе GitLab или GitHub независимо от того, инициализирован ли субмодуль локально.

Один URL не может одновременно вести на оба хоста, поэтому используется пара ссылок: **GitLab · GitHub**.

## wrtstat

| Документ | GitLab | GitHub |
|----------|--------|--------|
| Документация (`docs-md/README.md`) | [GitLab](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/docs-md/README.md?ref_type=heads) | [GitHub](https://github.com/mambaru/wrtstat/blob/master/docs-md/README.md) |
| Корневой Readme | [GitLab](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/Readme.md?ref_type=heads) | [GitHub](https://github.com/mambaru/wrtstat/blob/master/Readme.md) |
| Doxygen API | — | [mambaru.github.io/wrtstat](https://mambaru.github.io/wrtstat/index.html) |

Шаблон для inline-ссылки на документацию:

```markdown
[GitLab](https://gitlab.mamba.ru/cpp/wrtstat/-/blob/master/docs-md/README.md?ref_type=heads) · [GitHub](https://github.com/mambaru/wrtstat/blob/master/docs-md/README.md)
```
