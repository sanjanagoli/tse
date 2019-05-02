# Makefile for common
#
# Sanjana Goli, April 2019

L = ../libcs50

LIB = common.a
LLIBS = $L/libcs50.a
OBJS = pagedir.o index.o word.o
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc

all: $(LIB)

$(LIB): $(OBJS) $(LLIBS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h
index.o: index.h
word.o: index.h

.PHONY: all clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f *~ *.a
	rm -f $(LIB)