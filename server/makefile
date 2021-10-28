libs = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0 -lgio-2.0 -I/usr/include/mysql -lmysqlclient
output: server.o functionalities.o
	gcc -o serverBuild server.o functionalities.o $(libs)
functionalities.o: src/functionalities.c src/functionalities.h
	gcc -c src/functionalities.c $(libs)
server.o: src/server.c
	gcc -c src/server.c $(libs)
remove:
	rm *.o serverBuild