libs = lient src/client.c -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lglib-2.0 -lgio-2.0
target: 
	gcc -o $(libs)
	gcc -o $(libs)