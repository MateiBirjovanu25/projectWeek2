#include "client_actions.h"

GMutex mtx;

void* command_menu(void* arg){
    clientParam* cp = (clientParam*) arg;

      
    while(1){

        int param1;
        int param2;
        char command_number[10];
        int command;

 
        printf("Enter command number:\n");
        printf("1.receive from clientId (you have to insert it) \n");
        printf("2.receive script client_id\n");
        printf("3.exit\n");
        
        
        gets(command_number);
        command = sscanf(command_number,"%d %d",&param1,&param2); 
        if(command == 1)
        {
            printf("ok\n");
        }
        printf("command: %d\n",param1);
        printf("user id: %d\n",param2);
        GError* gerror=NULL;
        switch (param1)
        {
            case 1:
                printf("You will receive the file soon\n");
                char commandWithId[100];
                strcpy(commandWithId,"receive text");
                char idText[10];
                sprintf(idText,"!%d",param2);
                strcat(commandWithId,idText);
                if(g_socket_send(cp->socket,commandWithId,100,0,gerror) == -1)
                    printf("error");
                printf("command sent\n");
                receive_text(cp);
                break;
            
            case 2:
                printf("You will receive the script soon\n");
                g_socket_send(cp->socket, "receive text",100,0,0);
                g_socket_send(cp->socket, (gchar*)&param2, sizeof(int), 0,0);
                receive_script(cp->socket);
                break;
            
            case 3:
                g_socket_send(cp->socket, "exit",100,0,0);
                exit(1);
            
            default:
                printf("Can't solve the request\n");
                break;
        } 
    } 
    
}

void* respond(void* arg){
    clientParam* cp = (clientParam*) arg;

    while(1){
        char command[100];
        int debug;
        bzero(command,100);
        g_socket_receive(cp->socket,command,100,0,0);
        printf("received:\n %s\n",command);
        printf("from %d\n",cp->socket);
        if(strcmp(command,"send text")==0)  
        {
            send_text(cp);
        }   
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
    g_socket_send(socket, &clientType, 4, 0,0);//send the type of the client

    g_socket_send(socket,"active",100,0,0); //send the part of the client

    int client_id;
    g_socket_receive(socket, (gchar*)&client_id, sizeof(int), 0,0);//receive the client id
    printf("I am client number %d\n", client_id);


    g_mutex_init(&mtx);
    GThread* tid1;
    GThread* tid2;

    clientParam cP;
    cP.socket = socket;
    cP.address = address;
    cP.clientId = client_id;

    tid1 = g_thread_new(0,command_menu,&cP);
    tid2 = g_thread_new(0,respond,&cP);

    g_thread_join(tid1);
    g_thread_join(tid2);         
    g_mutex_clear(&mtx);
    
    return 0;
}