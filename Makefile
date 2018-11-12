# Makefile: PhotoLab3

#variable definitions
CC     = gcc
DEBUG  = -g -DDEBUG
NDEBUG = -O2 -DNDEBUG
CFLAGS = -Wall -ansi -std=c99 $(DEBUG) -c
LFLAGS = -Wall $(DEBUG)
DFLAGS = -Wall $(NDEBUG)
EFLAGS = -Wall -ansi -std=c99 $(NDEBUG) -c

#convenience targets
all : PhotoLab PhotoLabTest

clean: 
	rm -f *.o
	rm -f PhotoLab  
	rm -f PhotoLabTest

test: PhotoLab PhotoLabTest
	./PhotoLab ./PhotoLabTest

#2 programs to be compiled
PhotoLab: libFileIO.a libFilter.a PhotoLab2.o
	$(CC) $(DFLAGS) FileIO.o -lFileIO -L. DIPs.o Advanced.o -lFilter -L. PhotoLab2.o -o PhotoLab -lm

PhotoLabTest: libFileIO.a libFilter.a PhotoLab.o
	$(CC) $(LFLAGS) FileIO.o -lFileIO -L. DIPs.o Advanced.o -lFilter -L. PhotoLab.o -o PhotoLabTest -lm


#compilation rules
FileIO.o: FileIO.c FileIO.h Constants.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

DIPs.o: DIPs.c DIPs.h Constants.h
	$(CC) $(CFLAGS) DIPs.c -o DIPs.o

Advanced.o: Advanced.c Advanced.h Constants.h
	$(CC) $(CFLAGS) Advanced.c -o Advanced.o -lm

PhotoLab.o: PhotoLab.c Constants.h FileIO.h DIPs.h Advanced.h
	$(CC) $(CFLAGS) PhotoLab.c -o PhotoLab.o

PhotoLab2.o: PhotoLab.c Constants.h FileIO.h DIPs.h Advanced.h
	$(CC) $(EFLAGS) PhotoLab.c -o PhotoLab2.o

#libraries
libFilter.a: DIPs.o Advanced.o 
	ar rc libFilter.a DIPs.o Advanced.o
	ranlib libFilter.a

libFileIO.a: FileIO.o
	ar rc libFileIO.a FileIO.o
	ranlib libFileIO.a
