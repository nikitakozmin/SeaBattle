# Инструкция по сборке и запуску
Программа протестирована на Linux (Ubuntu 22.04.3 LTS) и Windows 11.

## Вариант №1: Через предустановленный Docker - рекомендуемый способ

### Сборка образа
> docker build -t seabattle .
### Запуск контейнера (с сохранениями глобально) - рекомендуемый способ
> docker run -it --rm -v "seabattle_saves:/usr/games/seabattle/bin/saves" --name running-seabattle seabattle
### Запуск контейнера (с сохранениями локально в контейнере) - альтернативный способ
> docker run -it --rm --name running-seabattle seabattle

## Вариант №2: Напрямую через свою ОС - альтернативный способ

### Установка самостоятельно:
- g++
- make

### Выполнение следующих действий:
В `src\Makefile`  
На Linux: Оставить EXE = `sbtl`  
На Windows: Изменить на EXE = `sbtl.exe`  

#### Ввести команды:
> make
---
Для Linux:
> ./sbtl
---
Для Windows:
> sbtl.exe
---

### Примечания:
Для очистки (лишних/промежуточных файлов) введите: `make clear` (после замены EXE в `src\Makefile`)
