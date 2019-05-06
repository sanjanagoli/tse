# Makefile for indexer
#
# Sanjana Goli, April 2019

L = ../libcs50
LL = ../common

PROG = indexer
PROGRAM = indextest
OBJS = indexer.o $(LL)/pagedir.o $(LL)/index.o $(LL)/word.o
OBJ = indextest.o $(LL)/pagedir.o $(LL)/index.o $(LL)/word.o
LIBS = -lm
LLIBS = $L/libcs50.a $(LL)/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(LL)
CC = gcc

all: indexer indextest

$(PROG): indexer.o $(LL)/pagedir.o $(LL)/index.o $(LL)/word.o $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

$(PROGRAM): $(OBJ) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJ) $(LLIBS) $(LIBS) -o $(PROGRAM)

indexer.o: $L/hashtable.h $L/counters.h $L/file.h $L/webpage.h $(LL)/index.h $(LL)/word.h $(LL)/pagedir.h 

indextest.o: $L/hashtable.h $L/counters.h $L/file.h $L/webpage.h $(LL)/index.h $(LL)/word.h $(LL)/pagedir.h 

.PHONY: all clean

test: 
	bash -v testing.sh

valgrind:
	valgrind bash -v testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer
	rm -f indextest