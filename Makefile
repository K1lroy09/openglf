CC = g++
EXECUTABLE = run
FLAGS = -lglut -lGL -lGLU -lGLEW
BIN = bin
LIB = lib
SRC = src
OBJ = $(BIN)/main.o

#GIT DETAILS
#GIT = N/A

#make rule
all: clean compile

#cleaning rule
.PHONY: clean
clean:
	rm -rf $(BIN)/*~ && rm -rf $(BIN)/*.o
	rm -rf $(SRC)/*~
	rm -rf *~
	rm -rf $(EXECUTABLE)
	
#compilation rules
compile: $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o $(EXECUTABLE)

$(BIN)/main.o: $(SRC)/main.cpp
	$(CC) $(SRC)/main.cpp -c -o $(BIN)/main.o

#runs the executable	
.PHONY: run
run:
	./$(EXECUTABLE)
	
#Shortcut for installation
.PHONY: install
install:
	sudo chmod -x install.sh
	sudo sh install.sh
	
#Git specific additions