CC=gcc

run: all
	nanoc.exe test.nano test.asm

all:
	$(CC) ./src/main.c -o nanoc.exe