BINFOLDER := bin/
INCFOLDER := inc/
SRCFOLDER := src/
OBJFOLDER := obj/
CC := gcc
CFLAGS := -lrt -pthread
SRCFILES := $(wildcard src/*.c)
all: $(SRCFILES:src/%.c=obj/%.o)
	$(CC) obj/*.o -o bin/exe $(CFLAGS)
obj/%.o: src/%.c
	$(CC) -c $< -o $@ -I./inc $(CFLAGS)
run: bin/exe
	bin/exe
.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf bin/*
