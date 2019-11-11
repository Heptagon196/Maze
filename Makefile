.SILENT:
FLAG = --std=c++17 -Dlinux
CPP = g++
DEPENDENCIES = ./src/Gos.o ./src/Error.o ./src/Any.o ./src/RPG.o ./src/Map.o ./src/main.o ./src/ConioPlus.o
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

ConsoleRPG: $(DEPENDENCIES)
	 echo "[8/8] Building ConsoleRPG"
	$(CPP) $(DEPENDENCIES) $(FLAG) -o ConsoleRPG

./src/Gos.o: ./src/Gos.cpp
	echo "[1/8] Compiling ./src/Gos.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Gos.o
./src/Error.o: ./src/Error.cpp
	echo "[2/8] Compiling ./src/Error.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Error.o
./src/Any.o: ./src/Any.cpp
	echo "[3/8] Compiling ./src/Any.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Any.o
./src/RPG.o: ./src/RPG.cpp
	echo "[4/8] Compiling ./src/RPG.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/RPG.o
./src/Map.o: ./src/Map.cpp
	echo "[5/8] Compiling ./src/Map.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/Map.o
./src/main.o: ./src/main.cpp
	echo "[6/8] Compiling ./src/main.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/main.o
./src/ConioPlus.o: ./src/ConioPlus.cpp
	echo "[7/8] Compiling ./src/ConioPlus.cpp"
	$(CPP) -c $(FLAG) $< -o ./src/ConioPlus.o

clean:
	echo "Cleaning files"
	rm -rf ./src/Gos.o ./src/Error.o ./src/Any.o ./src/RPG.o ./src/Map.o ./src/main.o ./src/ConioPlus.o
install:
	echo "Installing ConsoleRPG to /usr/local/bin"
	cp ./ConsoleRPG /usr/local/bin/
debug:
	echo "Building a debug version"
	env MAKE_DEBUG=true make ConsoleRPG
static:
	echo "Building a statically linked version"
	env MAKE_STATIC=true make ConsoleRPG
release:
	echo "Building a release version"
	env MAKE_RELEASE=true make ConsoleRPG
