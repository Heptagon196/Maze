CC=g++
FLAGS=--std=c++11 -Dlinux -lpthread
compile: Map.o
	$(CC) Map.o main.cpp -o ../Maze -O2 $(FLAGS)
Map.o:
	$(CC) -c Map.cpp
clean:
	rm *.o
	rm Maze
debug:
	$(CC) Map.o main.cpp -o ../Maze -g $(FLAGS)
