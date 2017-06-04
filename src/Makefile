CC=g++
FLAGS=--std=c++11 -Dlinux -O3
DEPEN=Map.o Conio.o Interpreter.o Cal.o InterpreterExt.o Bag.o Creature.o main.o
compile:$(DEPEN)
	$(CC) $(DEPEN) -o ../Maze $(FLAGS)
main.o:
	$(CC) -c main.cpp $(FLAGS)
Map.o:
	$(CC) -c Map.cpp $(FLAGS)
Conio.o:
	$(CC) -c Conio+.cpp -o Conio.o $(FLAGS)
Interpreter.o:
	$(CC) -c Interpreter.cpp -o Interpreter.o $(FLAGS)
Cal.o:
	$(CC) -c Cal.cpp -o Cal.o $(FLAGS)
InterpreterExt.o:
	$(CC) -c InterpreterExt.cpp -o InterpreterExt.o $(FLAGS)
Bag.o:
	$(CC) -c Bag.cpp -o Bag.o $(FLAGS)
Creature.o:
	$(CC) -c Creature.cpp -o Creature.o $(FLAGS)
clean:
	rm *.o -f
debug:
	$(CC) Map.o Conio.o main.cpp -o ../Maze -g $(FLAGS)
