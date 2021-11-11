#include "../include.h"

int clientID;

void* commands(void* arg)
{
    GSocket* socket = (GSocket*) arg;
    int command, targetID;

    printf("Enter a command:\n");
    printf("1.Receive from a client specified by ID \n");
    printf("2.Exit.\n");
    while(1){
    scanf("%d %d",&command, &targetID);
    if(command == 1)
    {
        printf("Receiving...\n");
        g_socket_send(socket,"receive text",100,0,0);
        g_socket_send(socket, (gchar*)&targetID, sizeof(int), 0,0);
        receive_text(socket);
    }
    else{
        if(command == 2)
            printf("Exit...\n");
        else
            printf("Command not found!\n");}
    }
}

void* respond(void* arg)
{

    GSocket* socket = (GSocket*) arg;
    while(1){
    char msg[20];
    bzero(msg, 20);
    g_socket_receive(socket, msg, 20, 0, 0);
    if(strcmp(msg, "send text") == 0)
        send_text(socket);
    }
}

int main(int argc,char** argv)
{
    struct sockaddr_in serveraddr;
    int PORT = atoi(argv[1]);

    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    GSocket* socket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP, NULL);
    GSocketAddress* address = g_socket_address_new_from_native(&serveraddr,sizeof(serveraddr));

    if(socket == 0)
        printf("Error creating the socket!\n");
    else
        printf("Socket created successfully!\n");

    if(g_socket_connect(socket,address, 0, 0) == 0)
    {
        printf("Connecting error! \n");
        exit(1);
    }
    else
        printf("Connected!\n");

    int client = 2;
    char clientType[20];
    g_socket_send(socket, (gchar*)&client, 4, 0, 0);
    g_socket_receive(socket, clientType, 20, 0, 0);
    clientID = atoi(clientType);

    GThread* t1;
    GThread* t2;

    t1 = g_thread_new(0,commands,socket);
    t2 = g_thread_new(0,respond,socket);

    g_thread_join(t1);
    g_thread_join(t2);  
    
    return 0;
}