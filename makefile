PROGRAM_NAME=erasure-streams-nameserver
SRC_DIR=src
BIN_DIR=bin

all: $(PROGRAM_NAME)

$(PROGRAM_NAME):
	mkdir -p $(BIN_DIR)
	cd $(SRC_DIR) && $(MAKE)

clean:
	rm -R $(BIN_DIR)
