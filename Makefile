
all: vectortest llisttest

%.o: %.c
	gcc -g -Wall -c $^

vectortest: vectortest.o vector.o
	gcc -g -Wall -o $@ $^

llisttest: llisttest.o llist.o
	gcc -g -Wall -o $@ $^

clean:
	rm -rf vectortest llisttest *.o
