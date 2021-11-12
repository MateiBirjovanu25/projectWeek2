#include "client_actions.h"

void send_text(GSocket* socket){

    char fileName[100] = "misc/text.in";
    //g_socket_receive(socket,fileName,100,0,0);
    
    int fd = open(fileName,O_RDONLY);
    char text[100];
    bzero(text,100);
    if(fd<0){
        strcpy(text, "Wrong path or file doesn't exist\n");
        printf("%s\n", text);
    }
    else{
        read(fd,text,100);
        printf("read: %s\n",text);
    }
        
    g_socket_send(socket,text,100,0,0);

}
void receive_text(GSocket* socket){

    char text[100];
    bzero(text,100);
    printf("started receiving\n");
    g_socket_receive(socket,text,100,0,0);

    printf("text received: \n");
    printf("%s\n",text);

}

void receive_script(GSocket* socket){

    //receive script and hashcode, check it and send it to update agent

    //g_socket_receive(socket, )
}