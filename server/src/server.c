#include "functionalities.h"
/*
input: void* parameter -> struct containing the sockets of the 2 clients
ouput: NULL
*/

void* resolveClient(void* a)
{
    clientPararameter* parameter = (clientPararameter*)a; 
    if(strcmp(parameter->command,"send") == 0)
    {
        sendText(parameter);
    }
    else if(strcmp(parameter->command,"receive") == 0)
    {
        receiveText(parameter);
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
        if(fork() == 0)
        {
            int clientType1,clientType2;
            char clientString[20];
            g_socket_receive(client1,clientString,4,0,0);
            clientType1 = atoi(clientString);
            g_socket_receive(client2,clientString,4,0,0);
            clientType2 = atoi(clientString);

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

            clientPararameter cp1;
            clientPararameter cp2;
            GThread* t1;
            GThread* t2;

            char text[1024];
            int done = 0;

            cp1.cl1 = client1;
            cp1.cl2 = client2;
            cp1.mtx = mtx;
            cp1.cond = cond;
            cp1.text = text;
            cp1.done = &done;
            strcpy(cp1.command,command1);


            cp2.cl1 = client2;
            cp2.cl2 = client1;
            cp2.mtx = mtx;
            cp2.cond = cond;
            cp2.text = text;
            cp2.done = &done;
            strcpy(cp2.command,command2);
            
            t1=g_thread_new(0,resolveClient,&cp1);
            t2=g_thread_new(0,resolveClient,&cp2); 

            g_thread_join(t1);
            g_thread_join(t2);


            g_mutex_clear(&mtx);
            g_cond_clear(&cond);
        }
    }
    
    
}





