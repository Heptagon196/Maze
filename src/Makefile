CC=g++
FLAGS=--std=c++11 -Dlinux -O3
DEPEN=Map.o Conio.o Interpreter.o Cal.o InterpreterExt.o
compile:$(DEPEN)
	$(CC) $(DEPEN) main.cpp -o ../Maze $(FLAGS)
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
clean:
	rm *.o -f
debug:
	$(CC) Map.o Conio.o main.cpp -o ../Maze -g $(FLAGS)
