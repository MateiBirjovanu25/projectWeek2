libs = -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lsodium -lz
target: client2.o client2_func.o
	gcc -Wall -o client2 client2.o client2_func.o $(libs)
client2_func.o: src/client2_func.c src/client2_func.h
	gcc -c src/client2_func.c $(libs)
client2.o: src/client2.c
	gcc -c src/client2.c $(libs)
remove:
	rm *.o client2