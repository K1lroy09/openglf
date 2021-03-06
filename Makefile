CC = g++
EXECUTABLE = run
FLAGS = -lglut -lGL -lGLU -lGLEW
BIN = bin
LIB = lib
SRC = src
OBJ = $(BIN)/main.o

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
compile: $(BIN)/main.o
	$(CC) $(BIN)/main.o $(FLAGS) -o $(EXECUTABLE)

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
.PHONY: git-init
git-init:
		git init

.PHONY: git-remote-add
git-remote-add:
		git remote add origin https://github.com/Arn3/openglf.git

.PHONY: git-push
git-push:
		git push origin master
		
