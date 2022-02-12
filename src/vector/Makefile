
CC=gcc
CFLAGS=-g -Wall

DIR=$(notdir $(CURDIR))
ROOT=../..
OBJ=$(ROOT)/obj/$(DIR)
BIN=$(ROOT)/bin

OBJS=$(OBJ)/vectortest.o $(OBJ)/vector.o
BINS=$(BIN)/vectortest

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
