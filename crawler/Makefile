# Makefile for crawler
#
# Sanjana Goli, April 2019

L = ../libcs50

PROG = crawler
OBJS = crawler.o
LIBS = -lm
LLIBS = $L/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc

all: $(PROG)

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $(PROG)

crawler.o: $L/hashtable.h $L/file.h $L/webpage.h

.PHONY: all clean

# test: $(PROG)
# 	./$(PROG)

# valgrind: $(PROG)
# 	valgrind ./$(PROG)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)