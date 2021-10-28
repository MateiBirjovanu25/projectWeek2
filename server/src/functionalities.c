#include "functionalities.h"
void sendRecvText(clientPar* clients)
{
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
        return ;
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
}