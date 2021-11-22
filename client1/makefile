libs = -pthread -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lsodium
target: client.o client_actions.o
	gcc -Wall -o client1 client.o client_actions.o $(libs)
client_actions.o: src/client_actions.c src/client_actions.h
	gcc -c src/client_actions.c $(libs)
client.o: src/client.c
	gcc -c src/client.c $(libs)
remove:
	rm *.o client1