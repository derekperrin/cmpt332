
CC = gcc
CFLAGS = -g
CPPFLAGS = -std=c99 -Wall -pedantic

.PHONY: all clean windows unix

OS = $(shell uname -s)

ifneq (,$(findstring MINGW,$(OS)))
TARGET = windows
else
TARGET = unix
endif

all: $(TARGET)

EXE += partA1.exe partA2 partA3 partA4

#compile part A
partA1: partA1.o
partA2: partA2.o
partA3: partA3.o
partA4: partA4.o

#compile part C
partC: mytestlist liblist.a

mytestlist: mytestlist.o liblist.a

liblist.a: liblist.a(list_adders.o list_movers.o list_removers.o)

windows: partA1

unix: partA2 partA3 partA4 partC

clean:
	$(RM) -rf $(EXE) *.o