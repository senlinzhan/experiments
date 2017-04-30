all: server client

client: client.o rio.o
	gcc $(CFLAGS) -o client client.o rio.o
client.o: client.c
	gcc $(CFLAGS) -c client.c
server: server.o rio.o
	gcc $(CFLAGS) -o server server.o rio.o
server.o: server.c
	gcc $(CFLAGS) -c server.c
rio.o: rio.c rio.h
	gcc $(CFLAGS) -c rio.c
clean:
	rm -f *.o server client
