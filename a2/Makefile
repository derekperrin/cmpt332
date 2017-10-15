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

EXE = $(BUILD_DIR)/s-chat reader-writer-test

ARCH = $(shell uname -m)
OS = $(shell uname -s)

# Mix tape side A
UBC_PTHREADS_DIR = /student/cmpt332/pthreads
UBC_PTHREADS_INC = -I$(UBC_PTHREADS_DIR)
UBC_PTHREADS_LD = -L$(UBC_PTHREADS_DIR)/lib/$(OS)$(ARCH)
UBC_PTHREADS_LIB = -lpthreads

# Mix tape side B
RT_THREADS_DIR = /student/cmpt332/rtt
RT_THREADS_INC = -I$(RT_THREADS_DIR)/include
RT_THREADS_LD = -L$(RT_THREADS_DIR)/lib/$(OS)$(ARCH)
RT_THREADS_LIB = -lRtt

# BUILD_DIR is the architecture specific directory path to be created
BUILD_DIR = build/$(OS)$(ARCH)
LIST_OBJS := $(addprefix $(BUILD_DIR)/, list_adders.o list_movers.o \
		list_alloc.o list_removers.o)
S_CHAT_OBJS := $(addprefix $(BUILD_DIR)/, s-chat.o)

.PHONY: all clean 

all: $(EXE)

# create the architecture specific directories
$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR)

# make the list library
$(BUILD_DIR)/liblist.a: $(LIST_OBJS) list.h | $(BUILD_DIR)
	$(AR) $(ARFLAGS) $@ $(LIST_OBJS)
# % does pattern matching to anything. This compiles each list object file.
$(BUILD_DIR)/list%.o: list%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

liblist.a: liblist.a(list_adders.o list_movers.o list_removers.o list_alloc.o)
list_movers.o: list_movers.c list.h
list_removers.o: list_movers.c list_alloc.c list_alloc.h list.h
list_adders.o: list_adders.c list_alloc.c list_alloc.h list.h
list_alloc.o: list_alloc.c list_alloc.h 

# make monitor library 
libMonitor.a: libMonitor.a(Monitor.o) liblist.a
Monitor.o: Monitor.c Monitor.h list.h 

reader-writer-test: LDFLAGS += $(UBC_PTHREADS_LD)
reader-writer-test: LDLIBS += $(UBC_PTHREADS_LIB) -lMonitor -llist 
reader-writer-test: CPPFLAGS += $(UBC_PTHREADS_INC)
reader-writer-test: reader-writer.o reader-writer-monitor.o libMonitor.a
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# No need to update CPPFLAGS because it is handled above to its dependencies
reader-writer.o: reader-writer.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^
reader-writer-monitor.o: reader-writer-monitor.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^

# Change variables just for s-chat and dependencies. 
$(BUILD_DIR)/s-chat: CFLAGS += -m32
$(BUILD_DIR)/s-chat: CPPFLAGS += $(RT_THREADS_INC)
$(BUILD_DIR)/s-chat: LDFLAGS += $(RT_THREADS_LD) -L./$(BUILD_DIR) -m32
$(BUILD_DIR)/s-chat: LDLIBS += $(RT_THREADS_LIB)

# The '|' character means: Do the following if it doesn't exist.
$(BUILD_DIR)/s-chat: $(S_CHAT_OBJS) $(BUILD_DIR)/liblist.a | $(BUILD_DIR)

$(S_CHAT_OBJS): s-chat.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) -rf $(EXE) *.o *.a build
