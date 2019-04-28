# Makefile for common
#
# Sanjana Goli, April 2019

L = ../libcs50

LIB = common.a
LLIBS = $L/libcs50.a
OBJS = pagedir.o
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc

all: $(LIB)

$(LIB): $(OBJS) $(LLIBS)
	ar cr $(LIB) $^

pagedir.o: $L/webpage.h pagedir.h

.PHONY: all clean

# test: $(PROG)
# 	./$(PROG)

# valgrind: $(PROG)
# 	valgrind ./$(PROG)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(LIB)