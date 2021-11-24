libs = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0 -lgio-2.0 -I/usr/include/mysql -lmysqlclient -lsodium
var:= $(wildcard src/*.c)
var:= $(filter-out src/server.c,$(Var))

output: parseText.o addToDatabase.o checkHash.o receiveText.o receiveScript.o server.o
	gcc -o serverBuild server.o parseText.o addToDatabase.o checkHash.o receiveText.o receiveScript.o $(libs)

server.o:src/server.c
	gcc -c src/server.c $(libs)

receiveText.o:: src/receiveText.c
	gcc -c src/receiveText.c $(libs)

receiveScript.o:: src/receiveScript.c
	gcc -c src/receiveScript.c $(libs)

addToDatabase.o:: src/addToDatabase.c
	gcc -c src/addToDatabase.c $(libs)

checkHash.o:: src/checkHash.c
	gcc -c src/checkHash.c $(libs)

parseText.o:: src/parseText.c
	gcc -c src/parseText.c $(libs)

remove:
	rm *.o serverBuild