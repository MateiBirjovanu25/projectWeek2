#include "functionalities.h"
/*
input: void* parameter -> struct containing the sockets of the 2 clients
ouput: NULL
*/
void* resolveClients(void* parameter)
{
    clientPar* clients=(clientPar*)(parameter);

    sendRecvText(clients);

    return NULL;
}


int main(int argc,char** argv)
{
    //dbTest();
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





