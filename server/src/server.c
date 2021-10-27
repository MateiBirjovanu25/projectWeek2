#include "../include.h"

typedef struct 
{
    GSocket* cl1;
    GSocket* cl2;
}clientPar;

/*
input: void* parameter -> struct containing the sockets of the 2 clients
ouput: NULL
*/
void* resolveClients(void* parameter)
{
    clientPar* clients=(clientPar*)(parameter);

    int clientType = 1; //sender
    g_socket_send(clients->cl1,&clientType,4,0,0);
    clientType = 2; //receiver
    g_socket_send(clients->cl2,&clientType,4,0,0);

    char request[100];
    g_socket_receive(clients->cl1,request,100,0,0);
    if(strcmp(request,"send") == 0)
    {}
    else if(strcmp(request,"receive") == 0)
    {
        GSocket* aux;
        aux = clients->cl1;
        clients->cl1 = clients->cl2;
        clients->cl2 = aux;
    }
    else
    {
        g_socket_send(clients->cl1,"invald input\n",100,0,0);
        g_socket_send(clients->cl2,"invald input\n",100,0,0);
        return NULL;
    }

    clientType = 1; //sender
    g_socket_send(clients->cl1,&clientType,4,0,0);
    clientType = 2; //receiver
    g_socket_send(clients->cl2,&clientType,4,0,0);

    g_socket_send(clients->cl1,"start sending\n",100,0,0);
    g_socket_send(clients->cl2,"receveing...\n",100,0,0);

    char text[1024];
    g_socket_receive(clients->cl1,text,1024,0,0);

    g_socket_send(clients->cl2,text,1024,0,0);

    return NULL;
}

int main(int argc,char** argv)
{
    int port = atoi(argv[1]);
    printf("%d\n",port);
    struct sockaddr_in server;
    server.sin_port=htons(port);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;

    GSocket* socket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);
    GSocketAddress* address = g_socket_address_new_from_native(&server,sizeof(server));

     if(g_socket_bind(socket,address,0,0)==0)
    {
        printf("bind error\n");
        exit(1);
    }

    g_socket_listen(socket,0);

    while(1)
    {
        GSocket* client1 = g_socket_accept(socket,0,0);
        GSocket* client2 = g_socket_accept(socket,0,0);
        printf("Clients connected\n");

        clientPar* parameter;
        parameter->cl1=client1;
        parameter->cl2=client2;

        GThread* t = g_thread_new(NULL,resolveClients,(void*)parameter);

        g_thread_join(t);

    }
        return 0;
}





