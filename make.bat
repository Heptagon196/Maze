@echo off
cd src
echo CC Conio+.cpp
g++ -c Conio+.cpp -o Conio.o --std=c++11
echo CC Map.cpp
g++ -c Map.cpp --std=c++11
echo CC main.cpp
g++ main.cpp Map.o Conio+.cpp -o ../Maze --std=c++11
cd ..
