CC=g++
FLAGS=--std=c++11 -Dlinux
compile: Map.o
	$(CC) Map.o main.cpp -o ../Maze -O2 $(FLAGS)
Map.o:
	$(CC) -c Map.cpp $(FLAGS)
clean:
	rm *.o
debug:
	$(CC) Map.o main.cpp -o ../Maze -g $(FLAGS)
