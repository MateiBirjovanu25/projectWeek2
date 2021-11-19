#include "serverActions.h"

void receiveText(activeClient* aC,int targetId)
{
    activeClient* clients = aC->activeClients;
    char text[100];
    printf("target socket: %d\n",clients[targetId].socket);
    printf("receiver socket: %d\n",aC->socket);
    GMutex mtx = aC->mutexes[targetId];
        printf("sending\n");
        g_socket_send(clients[targetId].socket,"send text",100,0,0);
        printf("sent receive request to %d\n",targetId);

        while(clients[targetId].secondSocket == NULL); //waiting for the client to create the second socket
        printf("second socket in thread: %d\n",clients[targetId].secondSocket);

        g_socket_receive(clients[targetId].secondSocket,text,100,0,0);

        //g_socket_close(clients[targetId].secondSocket,0);

        clients[targetId].secondSocket = NULL;

        printf("received text from target\n");

        while(clients[aC->id].secondSocket == NULL); //waiting for the client to create the second socket

        printf("started sending\n");

        g_socket_send(clients[aC->id].secondSocket,text,100,0,0);

        clients[aC->id].secondSocket = NULL;
        printf("sent\n");
}