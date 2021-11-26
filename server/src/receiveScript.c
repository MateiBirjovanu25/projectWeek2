#include "serverActions.h"

void receiveScript(activeClient* aC,int targetId)
{
    activeClient* clients = aC->activeClients;
    char text[1024],compressed[1024];
    printf("target socket: %d\n",clients[targetId].socket);
    printf("receiver socket: %d\n",aC->socket);
    printf("sending\n");
    g_socket_send(clients[targetId].socket,"send script",100,0,0);
    printf("sent receive request to %d\n",targetId);

    while(clients[targetId].secondSocket == NULL); //waiting for the target client to create the second socket
    printf("second socket in thread: %d\n",clients[targetId].secondSocket);

    g_socket_receive(clients[targetId].secondSocket,compressed,1024,0,0); //receive compressed file
    g_socket_receive(clients[targetId].secondSocket,text,1024,0,0); //receive hash | compressed hash


    // printf("script command: \n");
    // printf("%s\n",text);

    //g_socket_close(clients[targetId].secondSocket,0);


    if(checkHash(text,compressed) == 0) //check integrity of the file
        printf("succes\n");
    else
        printf("error\n");

    clients[targetId].secondSocket = NULL;

    while(clients[aC->id].secondSocket == NULL); //waiting for the receiving client to create the second socket


    printf("compressed: \n");
    for(int i=0;i<9;i++)
    {
        printf("%c",compressed[i]);
    }
    printf("\n");


    printf("started sending\n");

    g_socket_send(clients[aC->id].secondSocket,compressed,1024,0,0);
    g_socket_send(clients[aC->id].secondSocket,text,1024,0,0);

    clients[aC->id].secondSocket = NULL;
    printf("sent\n");
}