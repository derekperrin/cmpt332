#
# CMPT 332 -- Fall 2017
# Assignment 2
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
# Automatic variables from the make documentation:
# $@ -> target name
# $^ -> all prerequisites
# $< -> first prerequisite
# Make also has implicit variables such as CC. See Make docs for more info.

CC = gcc
CFLAGS = -g
CPPFLAGS = -Wall -pedantic -std=c90 -I./
LDFLAGS = -L./
LDLIBS = 
ARFLAGS = rcs

ARCH = $(shell uname -m)
OS = $(shell uname -s)
EXE := filesys-sim-threads

# Mixed tape side A
UBC_PTHREADS_DIR = /student/cmpt332/pthreads
UBC_PTHREADS_INC = -I$(UBC_PTHREADS_DIR)
UBC_PTHREADS_LD = -L$(UBC_PTHREADS_DIR)/lib/$(OS)$(ARCH)
UBC_PTHREADS_LIB = -lpthreads

.PHONY: all clean

all: $(EXE)

# Make the list library
liblist.a: liblist.a(list_adders.o list_movers.o list_removers.o list_alloc.o)
list_movers.o: list_movers.c list.h
list_removers.o: list_removers.c list.h
list_adders.o: list_adders.c list.h
list_alloc.o: list_alloc.c list_alloc.h

# Make the monitor library
libMonitor.a: libMonitor.a(Monitor.o) liblist.a
Monitor.o: Monitor.c Monitor.h list.h

# Filesystem implementation
filesys-sim-threads: LDFLAGS += $(UBC_PTHREADS_LD)
filesys-sim-threads: LDLIBS += $(UBC_PTHREADS_LIB) -lMonitor -llist 
filesys-sim-threads: CPPFLAGS += $(UBC_PTHREADS_INC)
filesys-sim-threads: filesys-sim-threads.o filesys-sim-monitor.o libMonitor.a
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	$(RM) -rf $(EXE) *.o *.a build




