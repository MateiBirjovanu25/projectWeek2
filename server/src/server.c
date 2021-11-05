#include "functionalities.h"

void* resolveClient(void* a)
{
    activeClient* aC = (activeClient*) a;
    char command[100]; //command received from client
    int targetId; //the id of the other client
    char* intBuffer[20]; //pointer used for receiving numbers
    g_mutex_init(&aC->mutexes[aC->id]);
    while(1)
    {
        g_socket_receive(aC->socket,command,100,0,0); // receive command
        g_socket_receive(aC->socket,intBuffer,20,0,0); //receive target id
        targetId = atoi(intBuffer);
        bzero(intBuffer,20);

        if(strcmp(command,"receive") == 0)
        {
            char text[1024];
            int done = 0;
            //receiveText(aC->activeClients[targetId],aC->mutexes[aC->id],text,&done);
            //sendText(aC->activeClients[aC->id],aC->mutexes[aC->id],text,&done);
            g_socket_send(aC->socket,"send",10,0,0);
            g_socket_receive(aC->socket,text,1024,0,0);
            g_socket_send(aC->socket,text,1024,0,0);
        }
        else
        {
            printf("command not found\n");
        }
    }
    return NULL;
}

int main(int argc,char** argv)
{
    //dbTest();
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
    int clientId = 0;
    while(1)
    {
        GSocket* client = g_socket_accept(socket,0,0);
        activeClient aC;
        aC.id = clientId;
        aC.socket = client;
        aC.activeClients = activeClients;
        aC.mutexes = mutexes;
        activeClients[clientId] = aC;
        clientId++;

        int clientType;
        char clientString[20];
        g_socket_receive(client,clientString,4,0,0);
        clientType = atoi(clientString);

        GThread* t1;

        t1=g_thread_new(0,resolveClient,&aC);

        g_thread_join(t1);

        g_socket_close(client,0);
    }
    
}





