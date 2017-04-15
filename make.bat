@echo off
cd src
g++ -c Map.cpp --std=c++11
g++ main.cpp Map.o -o ../Maze --std=c++11
cd ..
