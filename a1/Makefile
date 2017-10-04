# CMPT 332 -- Fall 2017
# Assignment 1
# Derek Perrin 		dmp450 11050915
# Dominic McKeith 	dom258 11184543
#
# Reminders:
# "target" means the part before the :
# "prerequisites" are the items after
# "recipe" is the line below target and prerequisites.
# -Make will detect files in the current directory and try to build them
#  automatically. Example, if partA1.c is present, when you put partA1.o
#  as a prerequisite for a target, make will find partA1.c and build it with
#  default flags.
# -If you don't include a recipe, make will build with default flags
# -If you don't include prerequisites for a target, make won't link them
#  together.
#
# Special variables from the make documentation:
# $@ -> target name
# $^ -> all prerequisites
# $< -> first prerequisite
# Make also has implicit variables such as CC. See Make docs for more info.

CC = gcc
CFLAGS = -g
CPPFLAGS = -std=c90 -Wall -pedantic
LDFLAGS = 
LDLIBS =
ARFLAGS = rcs

.PHONY: all clean windows unix

UBC_PTHREADS_DIR = /student/cmpt332/pthreads
ARCH = $(shell uname -m)
OS = $(shell uname -s)

# Add variables for compiling partA2 with UBC pthreads
UBC_PTHREADS_INC = -I$(UBC_PTHREADS_DIR)
UBC_PTHREADS_LD = -L$(UBC_PTHREADS_DIR)/lib/$(OS)$(ARCH)
UBC_PTHREADS_LIB = -lpthreads

# Check OS so you know what to compile for.
ifneq (,$(findstring MINGW,$(OS)))
TARGET = windows
else
TARGET = unix
endif

all: $(TARGET)

# this is used solely for cleaning
EXE += partA1.exe partA2 partA3 partA4 partE

# compile part A
partA1: partA1.o windows-common.o
partA2: partA2.o common.o
	$(CC) $(LDFLAGS) $(UBC_PTHREADS_LD) $^ $(LDLIBS) $(UBC_PTHREADS_LIB) \
		-o $@
partA3: partA3.o common.o
	$(CC) $(LDFLAGS) $^ -pthread $(LDLIBS) -o $@
partA4: partA4.o common.o

# need to specify include directory for partA2.o
partA2.o: partA2.c
	$(CC) $(CPPFLAGS) $(UBC_PTHREADS_INC) $(CFLAGS) -c $^

# compile part C
partC: mytestlist liblist.a

# compile part E
partE: partE.o liblist.a

# compiles mytestlist; temp sets LDFLAGS and LDLIBS to link liblist.a
mytestlist: LDFLAGS=-L.
mytestlist: LDLIBS=-llist
mytestlist: mytestlist.o liblist.a

# needed in case we make on Windows and then on Linux.
windows-common.o: common.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@
# add mytestlist to executables for cleaning
EXE += mytestlist

# Needed to add these so liblist.a would recompile if anything changed in
# these files. It also solves why list*.o files were being deleted at the end
# of make all.
list_adders.o: list_adders.c list.h
list_movers.o: list_movers.c list.h
list_removers.o: list_removers.c list.h
list_alloc.o: list_alloc.c list.h

# the parenthesis option are a way of using ar implicitely. See Make docs
liblist.a: liblist.a(list_adders.o list_movers.o list_removers.o list_alloc.o)

windows: partA1

unix: partA2 partA3 partA4 partC partE

clean:
	$(RM) -rf $(EXE) *.o *.a
