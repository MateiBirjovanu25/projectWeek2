#include "serverActions.h"

void* resolveClient(void* a)
{
    activeClient* aC = (activeClient*) a;
    char command[100]; //command received from client
    int targetId; //the id of the other client
    char intBuffer[20]; //pointer used for receiving numbers
    g_mutex_init(&aC->mutexes[aC->id]);
    printf("socket in thread: %d\n",aC->socket);
    while(1)
    {
        //printf("waiting for command\n");
        g_socket_receive(aC->socket,command,100,0,0); // receive command
        g_socket_receive(aC->socket,&targetId,4,0,0); //receive target id
        //printf("received: %s\n",command);
        if(strcmp(command,"receive text") == 0)
        {
            printf("receive request\n");
            receiveText(aC,targetId);
            printf("sent receive request to %d\n",targetId);
        }
        else if(strcmp(command,"exit") == 0)
        {
            return NULL;
        }
        else
        {
            //printf("command not found\n");
        }
    }
    return NULL;
}

int main(int argc,char** argv)
{
    int port = atoi(argv[1]);
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

    activeClient activeClients[100]; //array of active clients
    GMutex mutexes[100]; //array of mutexes(each client has its own mutex)
    int clientId = 1;
    g_mutex_init(&mutexes[0]);
    while(1)
    {
        GSocket* client = g_socket_accept(socket,0,0);
        printf("cient connected\n");
        activeClient aC;
        aC.id = clientId;
        aC.socket = client;
        aC.activeClients = activeClients;
        aC.mutexes = mutexes;

        activeClients[clientId] = aC;
        clientId++;
        int clientType;
        g_socket_receive(client,&clientType,4,0,0); //receive the type of the client
        printf("client %d\n",clientType);
        g_socket_send(client,&aC.id,sizeof(int),0,0);  //send the id to the client
        printf("socket in main: %d\n",client);
        GThread* t1;

        t1=g_thread_new(0,resolveClient,&aC);
    }
    g_mutex_clear(&mutexes[0]);
    return 0;
}





