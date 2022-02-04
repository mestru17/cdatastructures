
all: vectortest

%.o: %.c
	gcc -g -Wall -c $^

vectortest: vectortest.o vector.o
	gcc -g -Wall -o $@ $^

clean:
	rm -rf vectortest *.o *.dSYM
