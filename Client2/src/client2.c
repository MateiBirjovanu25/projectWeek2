#pragma once
#include <stdio.h>
#include <glib.h>
#include <sys/types.h>
#include <unistd.h>
#include <gio/gio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
//#include "../headers.h"

#define PORT 8080

void func(GSocket* sockfd)
{
    char msg[100];
    g_socket_receive(socket,msg,100,NULL,NULL);

    if(strcmp(msg, "send") == 0)
    {
        printf("Fisier text trimis catre server...\n");
        char buffer1[1024];
        FILE *fp = fopen("/home/calinnc/Documents/Exercises/Client1/misc/Mesaj", "r");
        while (fgets(buffer1, sizeof(buffer1), fp)) 
            g_socket_send(socket, buffer1, 1024, NULL, NULL);
        fclose(fp);
    }
    else 
        if(strcmp(msg, "receive") == 0)
        {
        char buffer2[1024];
        g_socket_receive(socket, buffer2, 1024, NULL, NULL);
        printf("Textul primit: \n");
        printf("%s\n", buffer2); 
        }
        else
            printf("Comanda primita nu este recunoscuta! Try again.\n");
}

int main(int argc,char** argv)
{
    struct sockaddr_in serveraddr;
    
    //creare socket si verificare
    GSocket* socket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP, NULL);
    GSocketAddress* address = g_socket_address_new_from_native(&serveraddr,sizeof(serveraddr));

    if(socket == NULL)
        printf("Eroare la crearea socket-ului!\n");
    else
        printf("Socket creat cu succes!\n");

    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(g_socket_connect(socket,address, 0, 0) == NULL)
    {
        printf("Conectare esuata! \n");
        exit(1);
    }
    else
        printf("Conectare reusita!\n");

    int client = 2;
    g_socket_send(socket, &client, 4, NULL, NULL);
    func(socket);

    return 0;
}
