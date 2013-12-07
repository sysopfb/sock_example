CC=c99
CFLAGS=-Wall

BINS = server client base.o

all: $(BINS)

base.o: base.c
	$(CC) -c base.c $(CFLAGS)

server: base.o server.c
	$(CC) -o server server.c base.o $(CFLAGS) -pthread

client: base.o client.c
	$(CC) -o client client.c base.o $(CFLAGS)

clean:
	rm $(BINS)
