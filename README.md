Савельев Егор Вячеславович
Б05-428а

## Запуск проекта
Сборка:

`git clone git@github.com:Savegorka/fl-2025-savelev-egor-task1.git &&
 cd fl-2025-savelev-egor-task1/ &&
 mkdir build &&
 cd build &&
 cmake .. &&
 make &&`
 
Чтобы запустить google тесты: 
 
`rm -rf build && 
 cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" -DCMAKE_EXE_LINKER_FLAGS="--coverage" &&
 cmake --build build -j &&
 ctest --test-dir build --output-on-failure &&
 gcovr -r . build --filter 'src/automatic.cpp' -e '.*tests.cpp' -e '.*main.cpp' -e '.*CMakeFiles.*' --branches --print-summary &&
 gcovr -r . build --filter 'src/automatic.cpp' -e '.*tests.cpp' -e '.*main.cpp' -e '.*CMakeFiles.*' --branches --html --html-details -o coverage.html`
 
Чтобы запустить ввод/вывод: `./main`
 
## Описание репозитория

Директория src содержит: 
- Файлы automatic.h/automatic.cpp - класс для работы с нка и функция, решающая задачу.
- Файл main.cpp - файл для ввода/вывода входных данных/ответа.

Директория tests содержит:
- Файл tests.cpp - тесты к коду

## [Описание алгоритма и доказательство асимптотики](ALGO.md)
