libs = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0 -lgio-2.0 -I/usr/include/mysql -lmysqlclient -lsodium
output: client.o client_actions.o
	gcc -o client1 client.o client_actions.o $(libs)
client_actions.o: src/client_actions.c src/client_actions.h
	gcc -c src/client_actions.c $(libs)
client.o: src/client.c
	gcc -c src/client.c $(libs)
remove:
	rm *.o client1