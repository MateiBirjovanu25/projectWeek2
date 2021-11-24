libs = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0 -lgio-2.0 -I/usr/include/mysql -lmysqlclient -lsodium
output: server.o serverActions.o
	gcc -o serverBuild server.o serverActions.o $(libs)
serverActions.o: src/serverActions.c src/serverActions.h
	gcc -c src/serverActions.c $(libs)
server.o: src/server.c
	gcc -c src/server.c $(libs)
remove:
	rm *.o serverBuild