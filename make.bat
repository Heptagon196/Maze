@echo off
cd src
echo CPP Conio+.cpp
g++ -c Conio+.cpp -o Conio.o --std=c++11
echo CPP Map.cpp
g++ -c Map.cpp --std=c++11
echo CPP Interpreter.cpp
g++ -c Interpreter.cpp --std=c++11
echo CPP InterpreterExt.cpp
g++ -c InterpreterExt.cpp --std=c++11
echo CPP Cal.cpp
g++ -c Cal.cpp --std=c++11
echo CPP main.cpp
g++ main.cpp Map.o Conio.o Interpreter.o InterpreterExt.o Cal.o -o ../Maze --std=c++11
cd ..
