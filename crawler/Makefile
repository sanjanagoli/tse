# Makefile for crawler
#
# Sanjana Goli, April 2019

L = ../libcs50
LL = ../common

PROG = crawler
OBJS = crawler.o
LIBS = -lm
LLIBS = $L/libcs50.a $(LL)/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(LL)
CC = gcc

all: $(PROG)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

crawler.o: $L/hashtable.h $L/file.h $L/webpage.h $(LL)/pagedir.h

.PHONY: all clean

test: 
	./testing.sh


removeTestFiles: 
	rm -rf testDir/*

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)