#include "../include.h"


void* command_menu(void* arg){

    GSocket* socket = (GSocket*) arg;

    int command_number;
    int from_client_id;

    printf("Enter command number:\n1.receive from clientId (you have to insert it) \n2.send\n");

    scanf("%d %d",&command_number, &from_client_id);

    switch (command_number)
    {
        case 1:
            printf("\nYou will receive the file soon");
            g_socket_send(socket,"receive text",100,0,0);
            g_socket_send(socket, (gchar*)&from_client_id, sizeof(int), 0,0);
            receive_text(socket);
            break;
        
        case 2:
            printf("\nThe file will be sent soon");
            g_socket_send(socket,"send text",100,0,0);
            send_text(socket);
            break;
        
        default:
            printf("Can't solve the request\n");
            break;
    }   
}

void* respond(void* arg){

    GSocket* socket = (GSocket*) arg;
    send_text(socket);
}

int main(int argc,char** argv)
{
    int port = atoi(argv[1]);
    struct sockaddr_in server;

    server.sin_port=htons(port);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1"); 

    GSocket* socket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);
    GSocketAddress* address = g_socket_address_new_from_native(&server,sizeof(server));

<<<<<<< HEAD
    while(1){
=======
    if(g_socket_connect(socket,address,0,0)==0)
    {
        printf("Eroare de conectare\n");
        exit(1);
    }
    else
        printf("Conectare efectuata cu succes!\n");

    //Client 1 decides if he sends or receives info
    int nr_client = 1;
    g_socket_send(socket,(gchar*)&nr_client,sizeof(int),0,0);
>>>>>>> cf840d0 (update multithread func)

        if(g_socket_connect(socket,address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }
        
        int client_id;
        g_socket_receive(socket, (gchar*)&client_id, sizeof(int), 0,0);

        GThread* tid1;
        GThread* tid2;

        tid1 = g_thread_new(0,command_menu,socket);
        tid2 = g_thread_new(0,respond,socket);

        //g_thread_join(tid1);
        //g_thread_join(tid2);         
    }
    
    return 0;
}