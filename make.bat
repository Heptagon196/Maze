@echo off
echo Building a release version
echo [1/9] Compiling ./src/Interpreter.cpp
g++ -c --std=c++11 -Dwindows -O2 ./src/Interpreter.cpp -o ./src/Interpreter.o
echo [2/9] Compiling ./src/Conio+.cpp
g++ -c --std=c++11 -Dwindows -O2 ./src/Conio+.cpp -o ./src/Conio+.o
echo [3/9] Compiling ./src/Cal.cpp
g++ -c --std=c++11 -Dwindows -O2 ./src/Cal.cpp -o ./src/Cal.o
echo [4/9] Compiling ./src/Bag.cpp
g++ -c --std=c++11 -Dwindows -O2 ./src/Bag.cpp -o ./src/Bag.o
echo [5/9] Compiling ./src/Creature.cpp
g++ -c --std=c++11 -Dwindows -O2 ./src/Creature.cpp -o ./src/Creature.o
echo [6/9] Compiling ./src/InterpreterExt.cpp
g++ -c --std=c++11 -Dwindows -O2 ./src/InterpreterExt.cpp -o ./src/InterpreterExt.o
echo [7/9] Compiling ./src/main.cpp
g++ -c --std=c++11 -Dwindows -O2 ./src/main.cpp -o ./src/main.o
echo [8/9] Compiling ./src/Map.cpp
g++ -c --std=c++11 -Dwindows -O2 ./src/Map.cpp -o ./src/Map.o
echo [9/9] Building Maze
g++ --std=c++11 -Dwindows -O2 ./src/Interpreter.o ./src/Conio+.o ./src/Cal.o ./src/Bag.o ./src/Creature.o ./src/InterpreterExt.o ./src/main.o ./src/Map.o -o Maze
