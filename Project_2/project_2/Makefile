CC = gcc
CFLAGS = -pthread -Wall

all: project2NEW.c

project2NEW.c: main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o project2NEW.c
