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
        printf("waiting for command\n");
        while(aC->working == true);
        aC->working = true;
        g_socket_receive(aC->socket,command,100,0,0); // receive command
        printf("received command: %s\n",command);
        targetId = parseText(command);
        printf("command: %s\n",command);
        printf("id: %d\n",targetId);
        if(strcmp(command,"receive text") == 0)
        {
            aC->checkArray[aC->id]++;
            printf("working: %d\n",aC->working);
            printf("received request\n");
            receiveText(aC,targetId);
        }
        else if(strcmp(command,"exit") == 0)
        {
            return NULL;
        }
        else
        {
            //printf("command not found\n");
        }
        aC->working = false;
        printf("working: %d\n",aC->working);
    }
    return NULL;
}

void* watchDog(void* param)
{
    watchDogParam* wParam = (watchDogParam*) param;
    int* clientId = wParam->clientId;
    int* checkArray = wParam->checkArray;
    GThread* threads = wParam->threads;
    activeClient* clients = wParam->clients;

    int oldValues[100];
    for(int i=0;i<100;i++)
        oldValues[i]=-1;
    while(1)
    {
        if(*clientId!=1)
        {
            sleep(5);
            for(int i=1;i<*clientId;i++)
            {   
                if(checkArray[i] == oldValues[i])
                {
                    printf("restarting\n");
                    g_thread_join(&threads[i]);
                    g_thread_new(0,resolveClient,&clients[i]);
                }    
                else
                {
                    oldValues[i] = checkArray[i];
                }
            }   
        }
    }    
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
    GThread threads[100]; //array used to store the thread of each client
    int checkArray[100]; //array use by the watchdog to check on the threads
    int clientId = 1;
    g_mutex_init(&mutexes[0]);

    watchDogParam wParam;
    wParam.checkArray = checkArray;
    wParam.threads = threads;
    wParam.clients = activeClients;
    wParam.clientId = &clientId;
    GThread* watchThread = g_thread_new(0,watchDog,&wParam);

    while(1)
    {
        GSocket* client = g_socket_accept(socket,0,0);
        printf("cient connected\n");
        activeClient aC;
        aC.id = clientId;
        aC.socket = client;
        aC.activeClients = activeClients;
        aC.mutexes = mutexes;
        aC.working = false;
        aC.checkArray = checkArray;

        checkArray[clientId] = -1;

        activeClients[clientId] = aC;
        clientId++;
        int clientType;
        g_socket_receive(client,&clientType,4,0,0); //receive the type of the client
        printf("client %d\n",clientType);
        g_socket_send(client,&aC.id,sizeof(int),0,0);  //send the id to the client
        printf("socket in main: %d\n",client);
        
        GThread* t;
        t = g_thread_new(0,resolveClient,&aC);

        threads[clientId-1] = *t;
    }
    g_mutex_clear(&mutexes[0]);
    return 0;
}





