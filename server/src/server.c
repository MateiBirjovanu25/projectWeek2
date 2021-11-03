#include "functionalities.h"
/*
input: void* parameter -> struct containing the sockets of the 2 clients
ouput: NULL
*/

char text[1024];
int done = 0;

void* resolveClient(void* a)
{
    clientPararameter* parameter = (clientPararameter*)a; 
    if(strcmp(parameter->command,"send") == 0)
    {
        sendText(parameter,text,&done);
    }
    else if(strcmp(parameter->command,"receive") == 0)
    {
        receiveText(parameter,text,&done);
    }
    else
    {
        printf("command not found\n");
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

    while(1)
    {
        GSocket* client1 = g_socket_accept(socket,0,0);
        GSocket* client2 = g_socket_accept(socket,0,0);
        printf("Clients connected\n");


        int clientType1,clientType2;
        g_socket_receive(client1,&clientType1,4,0,0);
        g_socket_receive(client2,&clientType2,4,0,0);

        //if client2 connected first, swap
        if(clientType1==2)
        {
            GSocket* aux;
            aux = client1;
            client1 = client2;
            client2 = aux;
        }

        
        char command1[100];
        char command2[100];

        g_socket_receive(client1,command1,100,0,0);
        g_socket_receive(client2,command2,100,0,0);

        printf("%s\n",command1);
        printf("%s\n",command2);


        if(strcmp(command2,command1) == 0)
        {
            g_socket_send(client1,"error",10,0,0);
            g_socket_send(client2,"error",10,0,0);
            continue;
        }
        g_socket_send(client2,"success",10,0,0);
        g_socket_send(client1,"success",10,0,0);

        GMutex mtx;
        g_mutex_init(&mtx);
        GCond cond;
        g_cond_init(&cond);

        clientPararameter* cp1 = malloc(sizeof(clientPararameter));
        clientPararameter* cp2 = malloc(sizeof(clientPararameter));
        cp1 -> cl1 = client1;
        cp1 -> cl2 = client2;
        cp1 -> mtx = mtx;
        cp1 -> cond = cond;
        strcpy(cp1->command,command1);

        GThread* t1;
        GThread* t2;
        

        cp2 -> cl1 = client2;
        cp2 -> cl2 = client1;
        cp2 -> mtx = mtx;
        cp2 -> cond = cond;
        strcpy(cp2->command,command2);
        


        g_thread_new(t1,resolveClient,cp1);
        g_thread_new(t2,resolveClient,cp2); 

        //g_thread_join(t1);
        //g_thread_join(t2);

        g_mutex_free(&mtx);
        g_cond_free(&cond);

    }
        return 0;
}





