CC = gcc
CFLAGS = -Wall -pedantic -std=c99

all: tictactoe

tictactoe: tictactoe.c ttt.h ttt1.o ttt2.o
	$(CC) $(CFLAGS) tictactoe.c ttt1.o ttt2.o -o tictactoe

ttt2.o: ttt2.c ttt.h
	$(CC) $(CFLAGS) -c ttt2.c

ttt1.o: ttt1.c ttt.h
	$(CC) $(CFLAGS) -c ttt1.c

clean:
	rm -i *.o tictactoe
