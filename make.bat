@echo off
echo Building a release version
echo [1/8] Compiling ./src/Gos.cpp
g++ -c --std=c++17 -Dwindows -O2 ./src/Gos.cpp -o ./src/Gos.o
echo [2/8] Compiling ./src/Error.cpp
g++ -c --std=c++17 -Dwindows -O2 ./src/Error.cpp -o ./src/Error.o
echo [3/8] Compiling ./src/Any.cpp
g++ -c --std=c++17 -Dwindows -O2 ./src/Any.cpp -o ./src/Any.o
echo [4/8] Compiling ./src/RPG.cpp
g++ -c --std=c++17 -Dwindows -O2 ./src/RPG.cpp -o ./src/RPG.o
echo [5/8] Compiling ./src/Map.cpp
g++ -c --std=c++17 -Dwindows -O2 ./src/Map.cpp -o ./src/Map.o
echo [6/8] Compiling ./src/main.cpp
g++ -c --std=c++17 -Dwindows -O2 ./src/main.cpp -o ./src/main.o
echo [7/8] Compiling ./src/ConioPlus.cpp
g++ -c --std=c++17 -Dwindows -O2 ./src/ConioPlus.cpp -o ./src/ConioPlus.o
echo [8/8] Building ConsoleRPG
g++ --std=c++17 -Dwindows -O2 ./src/Gos.o ./src/Error.o ./src/Any.o ./src/RPG.o ./src/Map.o ./src/main.o ./src/ConioPlus.o -o ConsoleRPG
