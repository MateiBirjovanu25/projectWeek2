#include "../include.h"


void* command_menu(void* arg){

    GSocket* socket = (GSocket*) arg;

    while(1){

        int command_number;
        int from_client_id;

        printf("Enter command number:\n");
        printf("1.receive from clientId (you have to insert it) \n");
        printf("2.send\n");

        scanf("%d %d",&command_number, &from_client_id);

        switch (command_number)
        {
            case 1:
                printf("You will receive the file soon\n");
                g_socket_send(socket,"receive text",100,0,0);
                g_socket_send(socket, (gchar*)&from_client_id, sizeof(int), 0,0);
                receive_text(socket);
                break;
            
            case 2:
                printf("The file will be sent soon\n");
                g_socket_send(socket,"send text",100,0,0);
                send_text(socket);
                break;
            
            default:
                printf("Can't solve the request\n");
                break;
        }  
    } 
}

void* respond(void* arg){

    GSocket* socket = (GSocket*) arg;

    while(1){
        char command[100];
        bzero(command,100);
        g_socket_receive(socket,command,100,0,0);

        if(strcmp(command,"send text")==0)  
            send_text(socket);
    }
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

    while(1){

        if(g_socket_connect(socket,address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }

        printf("Connected to server\n");
        
        g_socket_send(socket, (gchar*)CLIENT_ID, sizeof(CLIENT_ID),0,0);

        int client_id;
        g_socket_receive(socket, (gchar*)&client_id, sizeof(int), 0,0);
        printf("I am client number %d\n", client_id);

        GThread* tid1;
        GThread* tid2;

        tid1 = g_thread_new(0,command_menu,socket);
        tid2 = g_thread_new(0,respond,socket);

        g_thread_join(tid1);
        g_thread_join(tid2);         
    }
    
    return 0;
}