# ComputerClub

Сборка происходит при помощи инструмента cmake. 
Для сборки написан скрипт build.sh. Сборка приложения и работа способность приложения была протестирована на Linux. 

Для сборки под Windows необходимо выполнить следующий скрипт из корня репозитория.
```bash
cd src
cmake . -B=build-cmake-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-cmake-release
```
После его выполнения исполняемый файл будет лежать в директории src/cmake-build-release. 

Есть возможность локального запуска тестов. Для этого есть скрипт test.sh. Для него потребуется установленный vcpkg инструкция по его установке можно найти здесь https://learn.microsoft.com/ru-ru/vcpkg/get_started/get-started?pivots=shell-cmd (Интересует только первый пункт).
Так же в качестве первого аргумента скрипту нужно передать путь к vcpkg. Аналогично можно (попробовать сделать, работоспособность этого не была протестирована на windows).

Из корня репозитория запустить этот скрипт
```bash
cd tests
cmake  -B=build-cmake-release -DCMAKE_TOOLCHAIN_FILE="{path-to-vcpkg}/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build build-cmake-release
build-cmake-release/test.exe
```