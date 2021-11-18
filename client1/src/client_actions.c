#include "client_actions.h"

void send_text(clientParam* cp){

    char fileName[100] = "misc/text.in";
    //g_socket_receive(socket,fileName,100,0,0);
    
    int fd = open(fileName,O_RDONLY);
    char text[100];
    bzero(text,100);
    if(fd<0){
        strcpy(text, "Wrong path or file doesn't exist\n");
        printf("%s\n", text);
    }
    else{
        read(fd,text,100);
        printf("read: %s\n",text);
    }

    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);

    if(g_socket_connect(secondSocket,cp->address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }
    
    int clientType = 1; 
    g_socket_send(secondSocket, &clientType, 4, 0,0);//send the type of the client

    g_socket_send(secondSocket,"passive",100,0,0); //send the part of the client

    g_socket_send(secondSocket,&cp->clientId,4,0,0);//send the id of the client

    g_socket_send(secondSocket,text,100,0,0);

    system("clear");
    printf("Enter command number:\n");
    printf("1.receive from clientId (you have to insert it) \n");
    printf("2.receive script client_id\n");

    //g_socket_close(secondSocket,0);

}
void receive_text(clientParam* cp){

    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);

    if(g_socket_connect(secondSocket,cp->address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }
    
    int clientType = 1; 
    g_socket_send(secondSocket, &clientType, 4, 0,0);//send the type of the client

    g_socket_send(secondSocket,"passive",100,0,0); //send the part of the client

    g_socket_send(secondSocket,&cp->clientId,4,0,0);//send the id of the client

    char text[100];
    bzero(text,100);
    printf("started receiving\n");
    g_socket_receive(secondSocket,text,100,0,0);

    system("clear");
    printf("text received: \n");
    printf("%s\n",text);

}

void receive_script(clientParam* cp){
  
    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);

    if(g_socket_connect(secondSocket,cp->address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }
    
    int clientType = 1; 
    g_socket_send(secondSocket, &clientType, 4, 0,0);//send the type of the client

    g_socket_send(secondSocket,"passive",100,0,0); //send the part of the client

    g_socket_send(secondSocket,&cp->clientId,4,0,0);//send the id of the client

    char text[100];
    bzero(text,100);
    printf("started receiving\n");
    g_socket_receive(secondSocket,text,100,0,0);

    system("clear");
    printf("text received: \n");
    printf("%s\n",text);

    return NULL;
}