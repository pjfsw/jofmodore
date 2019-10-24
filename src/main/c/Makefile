CC := gcc
CFLAGS := --std=c99 -Wall -DMAC_OS_X_VERSION_MIN_REQUIRED=1770

EXEC := ../../../target/emu
OBJ := ../../../target/emu.o
LIBS := /Library/Frameworks/SDL2.framework/Versions/A/SDL2 /Library/Frameworks/SDL2_ttf.framework/SDL2_ttf


SRC := *.c *.h

$(OBJ):	$(SRC)
	$(CC) -o $(EXEC) $(CFLAGS) $(LIBS) $(SRC)