# A simple Makefile for compiling small SDL projects

# set the compiler
#CC := clang
CC := gcc

# set the compiler flags
#CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm
CFLAGS := --std=c99 -Wall

# add header files here
HDRS :=

TARGET := ../../../target

# add source files here
SRCS := emu.c instructions.c

# generate names of object files
OBJS := /Library/Frameworks/SDL2.framework/Versions/A/SDL2 /Library/Frameworks/SDL2_ttf.framework/SDL2_ttf $(SRCS:%.c=$(TARGET)/%.o)

# name of executable
EXEC := $(TARGET)/emu

# default recipe
all: $(EXEC)
 
#showfont: showfont.c Makefile
#	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

#glfont: glfont.c Makefile
#	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#    $(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean

