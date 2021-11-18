#include "client_actions.h"

#define MESSAGE ((const unsigned char *) "Arbitrary data to hash")
#define MESSAGE_LEN 22

void send_text(GSocket* socket){

    char fileName[100] = "misc/text.in";
    //g_socket_receive(socket,fileName,100,0,0);
    
    int fd = g_fopen(fileName,"r");
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
void receive_text(GSocket* socket){

    char text[1024];
    bzero(text,1024);
    g_socket_receive(socket,text,1024,0,0);

    printf("text received: \n");
    printf("%s\n",text);

}

void receive_script(GSocket* socket){

    //receive script and hashcode, check it and send it to update agent
    char script[2048];
    bzero(script, 2048);
    char hashcode[1024];

    g_socket_receive(socket,script,2048,0,0);
    g_socket_receive(socket, hashcode,1024,0,0);

    unsigned char hash[crypto_generichash_BYTES];

    crypto_generichash(hash, sizeof (hash),
                   script, sizeof(script),
                   NULL, 0);
    
    printf("%s\n",hash);
    printf("%s\n", hashcode);
    
    if(strcmp(hash, hashcode)!=0){
        printf("An error has occured while sending the script\n");
    }
    else{
        printf("The file will be sent to the update agent");
        //updateAgent.c apleaza o functie(param locatia fisierului) care face update
        FILE *fp;
        fp  = g_fopen ("data.sh", "w");

        fprintf(fp,"%s", script);
        fclose(fp);
    }
}