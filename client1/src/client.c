#include "client_actions.h"

#define RECEIVE_TEXT 1
#define RECEIVE_SCRIPT 2

GMutex *mtx;

void* command_menu(void* arg){

    GSocket* socket = (GSocket*) arg;

      
    while(1){

        int command_number;
        int param2;
       
        printf("Enter command number:\n");
        printf("1.receive from clientId (you have to insert it) \n");
        printf("2.receive script client_id\n");
      
        scanf("%d %d", &command_number, &param2);

        g_mutex_lock(&mtx); 

        switch (command_number)
        {
            case RECEIVE_TEXT:
                printf("You will receive the file soon\n");
                char text[20]="";
                sprintf(text, "%d", param2); 
                char command[100] = "receive text ";
                strcat(command,text);
                g_socket_send(socket, command,100,0,0);
                //g_socket_send(socket, (gchar*)&param2, sizeof(int), 0,0);
                receive_text(socket);
                break;
            
            case RECEIVE_SCRIPT:
                printf("You will receive the script soon");
                char text3[20]="";
                sprintf(text3, "%d", param2); 
                char command3[100] = "receive script ";
                strcat(command3,text3);
                g_socket_send(socket, command3,100,0,0);
                //g_socket_send(socket, (gchar*)&param2, sizeof(int), 0,0);
                receive_script(socket);
                break;
            
            default:
                printf("Can't solve the request\n");
                break;
        }  
        g_mutex_unlock(&mtx);   
    } 
    
}

void* respond(void* arg){

    GSocket* socket = (GSocket*) arg;
   
    while(1){

        g_mutex_lock(&mtx);
        char command[100];
        bzero(command,100);
        g_socket_receive(socket,command,100,0,0);
        //printf("command received: %s\n",command);

        if(strcmp(command,"send text")==0)  
        {
            printf("send\n");
            send_text(socket);
        }

        g_mutex_unlock(&mtx);   
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

    if(g_socket_connect(socket,address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }

    printf("Connected to server\n");

    int clientType = 1;
    g_socket_send(socket, &clientType, 4, 0,0);

    int client_id;  //structura client id si socket
    g_socket_receive(socket, (gchar*)&client_id, sizeof(int), 0,0);
    printf("I am client number %d\n", client_id);

    g_mutex_init(&mtx);
    GThread* tid1;
    GThread* tid2;

    tid1 = g_thread_new(0,command_menu,socket);
    tid2 = g_thread_new(0,respond,socket);

    g_thread_join(tid1);
    g_thread_join(tid2);         
    g_mutex_clear(&mtx);
    
    return 0;
}