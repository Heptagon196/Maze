CC=g++
FLAGS=--std=c++11 -Dlinux
compile: Map.o Conio.o
	$(CC) Map.o Conio.o main.cpp -o ../Maze -O2 $(FLAGS)
Map.o:
	$(CC) -c Map.cpp $(FLAGS)
Conio.o:
	$(CC) -c Conio+.cpp -o Conio.o $(FLAGS)
clean:
	rm *.o
debug:
	$(CC) Map.o Conio.o main.cpp -o ../Maze -g $(FLAGS)
