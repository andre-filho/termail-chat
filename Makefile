BINFOLDER := bin/
INCFOLDER := inc/
SRCFOLDER := src/
OBJFOLDER := obj/
CC := gcc
CFLAGS := -lrt -pthread
SRCFILES := $(wildcard src/*.c)

.PHONY: clean all run fresh

all: $(SRCFILES:src/%.c=obj/%.o)
	$(CC) obj/*.o -o bin/exe $(CFLAGS)

obj/%.o: src/%.c
	$(CC) -c $< -o $@ -I./inc $(CFLAGS)

clean:
	rm -rf obj/*
	rm -rf bin/*

run: bin/exe
	bin/exe
