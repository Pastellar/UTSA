CC = gcc -pthread
CFLAGS = -Wall -g

all: prog

clean:
	rm -f *.o prog

helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c

prog.o: prog.c helper.h
	$(CC) $(CFLAGS) -c prog.c

prog: prog.o helper.o
	$(CC) $(CFLAGS) -o prog prog.o helper.o
