#include "../include.h"

void send_text(GSocket* socket){

    char fileName[100];
    g_socket_receive(socket,fileName,100,0,0);
    
    int fd = open(fileName,O_RDONLY);
    char text[1024];
    bzero(text,1024);
    if(fd<0){
        strcpy(text, "Wrong path or file doesn't exist\n");
        printf("%s\n", text);
    }
    else{
        read(fd,text,1024);
    }
        
    g_socket_send(socket,text,1024,0,0);

}
void receive_text(GSocket* socket, char *filepath){

    char text[1024];
    bzero(text,1024);
    g_socket_send(socket, filepath, strlen(filepath),0,0);
    g_socket_receive(socket,text,1024,0,0);

    printf("text received: \n");
    printf("%s\n",text);

}

int main(int argc,char** argv)
{
    int port = atoi(argv[1]);
    struct sockaddr_in server;
    //GError *error = NULL;

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

        //Client 1 decides if he sends or receives info
        int nr_client = 1;
        g_socket_send(socket,(gchar*)&nr_client,sizeof(int),0,0);

        char request[100];
        printf("Enter command number:\n1.receive filename \n2.send\n");
        scanf("%[^\n]",request);

        char command[8];
        char filepath[30];

        char * pch;
        int i = 0;
        pch = strtok (request," \n");
        while (pch != NULL)
        {
            if(i == 0){
                strcpy(command,pch);
            }
            if( i == 1)
                strcpy(filepath, pch);
            i++;
            pch = strtok (NULL, " \n");
        }

        if(i < 1 && strcmp(command,"receive")==0){
            printf("Wrong number of arguments\n");
        }

        g_socket_send(socket,command,100,0,0);


        char message[100];
        g_socket_receive(socket, message,100,0,0);

        if(strcmp(message, "success")==0){
            if(strcmp(command,"send")==0){
                    send_text(socket);
                    strcpy(request, "\n");
                }       
                else{
                    receive_text(socket, filepath);
                    strcpy(request, "\n");
                }
        }
        else
            printf("Can't solve the request\n");
        
    }
    

    return 0;
}