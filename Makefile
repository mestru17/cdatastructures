
ROOT=$(CURDIR)
SRC=$(ROOT)/src
DIRS=vector llist

all:
	for dir in $(DIRS); do\
		cd $(SRC)/$$dir; make;\
	done

clean:
	for dir in $(DIRS); do\
		cd $(SRC)/$$dir; make clean;\
	done
