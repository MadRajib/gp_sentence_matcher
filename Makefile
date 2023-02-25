CC=gcc
CFLAGS=-g

word_finder: finder_main.c
	$(CC) $(CFLAGS) -o word_finder finder_main.c
