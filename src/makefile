OUT=erasure-streams-nameserver
COMPILER=gcc
FLAGS=-g -Wall -pedantic
LIBS=-lwebsockets -lpthread

BIN_DIR=../bin

all: $(OUT)

$(OUT):name_server.o intern.o llist.o redirector.o
	$(COMPILER) $(FLAGS) -o $(BIN_DIR)/$(OUT) $(BIN_DIR)/name_server.o $(BIN_DIR)/intern.o $(BIN_DIR)/llist.o $(BIN_DIR)/redirector.o $(LIBS)

name_server.o: name_server.c name_server.h intern.h
	$(COMPILER) $(FLAGS) -o $(BIN_DIR)/name_server.o name_server.c -c

intern.o: intern.c intern.h llist.h
	$(COMPILER) $(FLAGS) -o $(BIN_DIR)/intern.o intern.c -c

llist.o: llist.c llist.h 
	$(COMPILER) $(FLAGS) -o $(BIN_DIR)/llist.o llist.c -c

redirector.o: redirector.c redirector.h
	$(COMPILER) $(FLAGS) -o $(BIN_DIR)/redirector.o redirector.c -c