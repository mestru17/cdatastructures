
CC=gcc
CFLAGS=-g -Wall

ROOT=../..
OBJ=$(ROOT)/obj/llist
BIN=$(ROOT)/bin

OBJS=$(OBJ)/llisttest.o $(OBJ)/llist.o
BINS=$(BIN)/llisttest

all: $(BINS)

$(BINS): $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o: %.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $@

$(BIN):
	mkdir -p $@

clean:
	rm -rf $(OBJ) $(BINS)
