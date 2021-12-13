target: network.o libnet.so
	gcc -Wall -o network network.o
network.o: src/network.c network.h
	gcc -c -Wall -Werror -fpic src/network.c
libnet.so: src/network.c
	gcc -shared -o libnet.so src/network.c