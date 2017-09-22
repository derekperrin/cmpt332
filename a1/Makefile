# CMPT 332 -- Fall 2017
# Assignment 1
# Derek Perrin 		dmp450 11050915
# Dominic McKeith 	dom258 11184543

CC = gcc
CFLAGS = -g
CPPFLAGS = -std=c99 -Wall -pedantic
LDFLAGS = 
LDLIBS =
ARFLAGS = rcs

.PHONY: all clean windows unix

UBC_PTHREADS_DIR = /student/cmpt332/pthreads
ARCH = $(shell uname -m)
OS = $(shell uname -s)

UBC_PTHREADS_INC = -I$(UBC_PTHREADS_DIR)
UBC_PTHREADS_LD = -L$(UBC_PTHREADS_DIR)/lib/$(OS)$(ARCH)
UBC_PTHREADS_LIB = -lpthreads

ifneq (,$(findstring MINGW,$(OS)))
TARGET = windows
else
TARGET = unix
endif

all: $(TARGET)

EXE += partA1.exe partA2 partA3 partA4

# compile part A
partA1: partA1.o common.o
partA2: partA2.o common.o
	$(CC) $(LDFLAGS) $(UBC_PTHREADS_LD) $^ $(LDLIBS) $(UBC_PTHREADS_LIB) \
		-o $@
partA3: partA3.o
	$(CC) $(LDFLAGS) $^ -pthread $(LDLIBS) -o $@
partA4: partA4.o

# need to specify include directory for partA2.o
partA2.o: partA2.c
	$(CC) $(CPPFLAGS) $(UBC_PTHREADS_INC) $(CFLAGS) -c $^

# compile part C
partC: mytestlist liblist.a

# compiles mytestlist; temp sets LDFLAGS and LDLIBS to link liblist.a 
mytestlist: LDFLAGS=-L.
mytestlist: LDLIBS=-llist
mytestlist: mytestlist.o liblist.a

# add mytestlist to executables for cleaning
EXE += mytestlist

liblist.a: liblist.a(list_adders.o list_movers.o list_removers.o list_alloc.o)

windows: partA1

unix: partA2 partA3 partA4 partC

clean:
	$(RM) -rf $(EXE) *.o *.a
