.SILENT:
FLAG = --std=c++11 -Dlinux
CPP = g++
DEPENDENCIES = ./src/Interpreter.o ./src/Conio+.o ./src/Cal.o ./src/Bag.o ./src/Creature.o ./src/InterpreterExt.o ./src/main.o ./src/Map.o
ifdef MAKE_RELEASE
	FLAG += -O2
endif
ifdef MAKE_DEBUG
	FLAG += -DDEBUG -g
endif
ifdef MAKE_STATIC
	FLAG += -static
endif

default:
	make release

Maze: $(DEPENDENCIES)
	 echo "[9/9] Building Maze"
	$(CPP) $(DEPENDENCIES) $(FLAG) -o Maze

./src/Interpreter.o: ./src/Interpreter.cpp
	echo "[1/9] Compiling ./src/Interpreter.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Interpreter.o
./src/Conio+.o: ./src/Conio+.cpp
	echo "[2/9] Compiling ./src/Conio+.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Conio+.o
./src/Cal.o: ./src/Cal.cpp
	echo "[3/9] Compiling ./src/Cal.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Cal.o
./src/Bag.o: ./src/Bag.cpp
	echo "[4/9] Compiling ./src/Bag.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Bag.o
./src/Creature.o: ./src/Creature.cpp
	echo "[5/9] Compiling ./src/Creature.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Creature.o
./src/InterpreterExt.o: ./src/InterpreterExt.cpp
	echo "[6/9] Compiling ./src/InterpreterExt.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/InterpreterExt.o
./src/main.o: ./src/main.cpp
	echo "[7/9] Compiling ./src/main.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/main.o
./src/Map.o: ./src/Map.cpp
	echo "[8/9] Compiling ./src/Map.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Map.o

clean:
	echo "Cleaning files"
	rm -rf ./src/Interpreter.o ./src/Conio+.o ./src/Cal.o ./src/Bag.o ./src/Creature.o ./src/InterpreterExt.o ./src/main.o ./src/Map.o
install:
	echo "Installing Maze to /usr/local/bin"
	cp ./Maze /usr/local/bin/
debug:
	echo "Building a debug version"
	env MAKE_DEBUG=true make Maze
static:
	echo "Building a statically linked version"
	env MAKE_STATIC=true make Maze
release:
	echo "Building a release version"
	env MAKE_RELEASE=true make Maze
