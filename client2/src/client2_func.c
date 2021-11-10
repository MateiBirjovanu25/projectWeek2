#include "client2_func.h"

void sendText(GSocket* socket)
{
    printf("File sent to the server...\n");
    char fileName[100] = "misc/mesaj";
    int fd = open(fileName,O_RDONLY);
    char buffer[1024];
    bzero(buffer, 1024);
    read(fd,buffer,1024);
    g_socket_send(socket,buffer,1024,0,0);
}

void receiveText(GSocket* socket)
{
        char buffer[1024];
        bzero(buffer, 1024);
        g_socket_receive(socket, buffer, 1024, 0, 0);
        printf("Text received: \n");
        printf("%s\n", buffer); 
}

void sendScript(GSocket* socket){}