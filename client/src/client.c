#include "../include.h"

void dbTest()
{
    
}


int main(int argc,char** argv)
{
    dbTest();
    int port = atoi(argv[1]);
    struct sockaddr_in server;

    server.sin_port=htons(port);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1"); 

    GSocket* socket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);
    GSocketAddress* address = g_socket_address_new_from_native(&server,sizeof(server));

    if(g_socket_connect(socket,address,0,0)==0)
    {
        printf("Connect error\n");
        exit(1);
    }

    //Client 1 decides if he sends or receives info
    int nr_client = 0;
    g_socket_send(socket,&nr_client,4,0,0);

    char request[100];
    //printf("Logged in -> enter command: send or receive\n");
    //scanf("%s",request);
    //g_socket_send(socket,request,100,0,0);

    //char message[100];
    g_socket_receive(socket,request,100,0,0);
    //printf("%s",message);

    if(strcmp(request,"send")==0)
    {
        printf("enter file name: \n");
        char fileName[100];
        scanf("%s",fileName);
        int fd = open(fileName,O_RDONLY);
        char text[1024];
        read(fd,text,1024);
        g_socket_send(socket,text,1024,0,0);
    }
    else
    {
        char text[1024];
        g_socket_receive(socket,text,1024,0,0);
        printf("text received: \n");
        printf("%s\n",text);
    }
    return 0;
}