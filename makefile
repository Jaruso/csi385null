CC     = gcc
CFLAGS = -ansi -std=c99 -g -c

pbs.o: bootSector.h
	${CC} ${CFLAGS} pbs.c
