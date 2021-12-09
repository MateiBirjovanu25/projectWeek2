target: network.o net.so
	gcc -Wall -o network network.o
network.o: src/network.c network.h
	gcc -c -Wall -Werror -fpic src/network.c
net.so: src/network.c
	gcc -shared -o net.so src/network.c