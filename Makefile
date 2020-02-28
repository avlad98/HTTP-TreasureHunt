CC=g++
CFLAGS=-Wno-write-strings -I.

client: client.cpp requests.cpp helpers.cpp parson.c
		$(CC) -o client client.cpp requests.cpp helpers.cpp parson.c -Wall
		
parson: parson.c
		gcc -o parson.c -Wall

run: client
		./client

clean:
		rm -f *.o client
