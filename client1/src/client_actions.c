#include "client_actions.h"

void send_text(clientParam* cp){

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

    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);

    if(g_socket_connect(secondSocket,cp->address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }
    
    int clientType = 1; 
    g_socket_send(secondSocket, &clientType, 4, 0,0);//send the type of the client

    g_socket_send(secondSocket,"passive",100,0,0); //send the part of the client

    g_socket_send(secondSocket,&cp->clientId,4,0,0);//send the id of the client

    g_socket_send(secondSocket,text,100,0,0);

    system("clear");
    printf("Enter command number:\n");
    printf("1.receive from clientId (you have to insert it) \n");
    printf("2.receive script client_id\n");
    printf("3.exit\n");

    //g_socket_close(secondSocket,0);

}
void receive_text(clientParam* cp){

    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);

    if(g_socket_connect(secondSocket,cp->address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }
    
    int clientType = 1; 
    g_socket_send(secondSocket, &clientType, 4, 0,0);//send the type of the client

    g_socket_send(secondSocket,"passive",100,0,0); //send the part of the client

    g_socket_send(secondSocket,&cp->clientId,4,0,0);//send the id of the client

    char text[100];
    bzero(text,100);
    printf("started receiving\n");
    g_socket_receive(secondSocket,text,100,0,0);

    system("clear");
    printf("text received: \n");
    printf("%s\n",text);

}

void receive_script(clientParam* cp){
  
    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);

    if(g_socket_connect(secondSocket,cp->address,0,0)==0)
        {
            printf("Connecting error\n");
            exit(1);
        }
    
    int clientType = 1; 
    g_socket_send(secondSocket, &clientType, 4, 0,0);//send the type of the client

    g_socket_send(secondSocket,"passive",100,0,0); //send the part of the client

    g_socket_send(secondSocket,&cp->clientId,4,0,0);//send the id of the client

    char compressed_script_hash[1024];
    bzero(compressed_script_hash,1024);

    printf("started receiving\n");
    g_socket_receive(secondSocket,compressed_script_hash,1024,0,0);

    //compressedFile ! hascodeCompressed ! hashcodeDecompressed

    char hashcode_compressed[1024];
    char compressed[1024];
    char hashcode_decompressed[1024];


    int n = strlen(compressed_script_hash) - 2*(crypto_generichash_BYTES) + 1;
    int mid = n + crypto_generichash_BYTES;
    int k=0;
    int l=0;

    strncpy(compressed, compressed_script_hash, n);

    for(int i = n; i < strlen(compressed_script_hash); i++)
    {
        if(i < mid){
            hashcode_compressed[k] = compressed_script_hash[i];
            k++;
        }
        else
        {
            hashcode_decompressed[l] = compressed_script_hash[i];
            l++;
        }
    }


    unsigned char hash[crypto_generichash_BYTES];

    crypto_generichash(hash, sizeof (hash),
                   compressed, sizeof(compressed),
                   NULL, 0);
    
    printf("%s\n",hash);
    printf("%s\n", hashcode_compressed);
    
    if(strcmp(hash, hashcode_compressed)!=0){
        printf("An error has occured while sending the script - compressed.\n");
    }
    else{
        
        //decompres
        //compar hash de pe file decompresat
        unsigned char hash[crypto_generichash_BYTES];
        char decompressed[1024];
        
        crypto_generichash(hash, sizeof (hash),
                   decompressed, sizeof(decompressed),
                   NULL, 0);

        if(strcmp(hash, hashcode_decompressed)!=0){
            printf("An error has occured while sending the script - decompressed.\n");
        }
        else
        {
            printf("The file will be sent to the update agent");
        }

        
        //updateAgent.c apleaza o functie(param locatia fisierului) care face update
        //FILE *fp;
        //fp  = fopen ("data.sh", "w");

        //fprintf(fp,"%s", script);
        //fclose(fp);
    }

    system("clear");
    printf("script received: \n");

    return NULL;
}

char* decompress(char *compressed){
    char decompressed[1024];
    
    return decompressed;
}

void extract_string_2(){

    char message[100]= "ANAA";
    char hashcode_compressed[100];
    char compressed[100];
    char hashcode_decompressed[100];

    int n = strlen(message) - 2*(crypto_generichash_BYTES) + 1;
    int mid = n + crypto_generichash_BYTES;
    int k=0;
    int l=0;

    strncpy(compressed, message, n);

    for(int i = n; i < strlen(message); i++)
    {
        if(i < mid){
            hashcode_compressed[k] = message[i];
            k++;
        }
        else
        {
            hashcode_decompressed[l] = message[i];
            l++;
        }
    }
}

void test_extract_string(){

    char script[100]= "ANAA";

    char hashcode[100];

    unsigned char hash[crypto_generichash_BYTES];
    unsigned char hash2[crypto_generichash_BYTES];

    crypto_generichash(hash, sizeof (hash),
                   script, sizeof(script),
                   NULL, 0);

    printf("%d %d\n", strlen(hash), crypto_generichash_BYTES);


    crypto_generichash(hash, sizeof hash,
                   script, sizeof(script),
                   NULL, 0);
    
    crypto_generichash(hash2, sizeof hash2,
                   script, sizeof(script),
                   NULL, 0);
    
    printf("%d\n",strlen(hash));
    printf("%d\n",strlen(hash2));
    
}
