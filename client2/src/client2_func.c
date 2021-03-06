#include "client2_func.h"

void sendText(clientParam* cp)
{
    printf("File sent to the server...\n");
    char fileName[100] = "misc/mesaj";
    int fd = open(fileName,O_RDONLY);
    char buffer[1024];
    bzero(buffer, 1024);
    if(fd < 0)
    {
        printf("Wrong path or file doesn t exist!\n");
    }
    else
        read(fd,buffer,1024);
    
    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP, NULL);
    if(g_socket_connect(secondSocket, cp->addr,0,0) == 0)
    {
        printf("Connecting error..\n");
        exit(1);
    }
    int clientType = 2;

    g_socket_send(secondSocket, &clientType,4,0,0);
    g_socket_send(secondSocket, "passive", 100,0,0);
    g_socket_send(secondSocket, &cp->clientID,4,0,0);
    g_socket_send(secondSocket, buffer,1024,0,0);
}

void receiveText(clientParam* cp)
{
    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4,G_SOCKET_TYPE_STREAM,G_SOCKET_PROTOCOL_TCP,NULL);
    if(g_socket_connect(secondSocket, cp->addr,0,0) == 0)
    {
        printf("Connecting error..\n");
        exit(1);
    }
    int clientType = 2;
    g_socket_send(secondSocket, &clientType, 4, 0,0);
    g_socket_send(secondSocket, "passive",100,0,0); 
    g_socket_send(secondSocket, &cp->clientID,4,0,0);

    char buffer[1024];
    bzero(buffer, 1024);
    g_socket_receive(secondSocket, buffer, 1024, 0, 0);
    printf("Text received: \n");
    printf("%s\n", buffer); 
}

void sendScript(clientParam* cp)
{
    printf("Script sent to the server...\n");
    char scriptName[100] = "misc/script.sh";
    int fd = open(scriptName,O_RDONLY);
    char originalBuff[1024],compressedBuff[1024], originalHash[256],compressedHash[256];
    bzero(originalBuff, 1024);
    if(fd < 0)
    {
        printf("Wrong path or script doesn t exist!\n");
    }
    else
        read(fd, originalBuff, 1024);

    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, NULL);
    if(g_socket_connect(secondSocket, cp->addr,0,0) == 0)
    {
        printf("Connecting error..\n");
        exit(1);
    }
    int clientType = 2;
    //generate compressed txt and hash, then concatenate original txt, compr hash and original hash.
    generateHash(originalBuff, originalHash);
    compressTxt(originalBuff,compressedBuff);
    generateHash(compressedBuff, compressedHash);
    strcat(compressedBuff, compressedHash);
    strcat(compressedBuff, originalHash);

    g_socket_send(secondSocket, &clientType, 4, 0, 0);
    g_socket_send(secondSocket, "passive", 100, 0, 0);
    g_socket_send(secondSocket, &cp->clientID, 4, 0, 0);
    g_socket_send(secondSocket, compressedBuff, 1024, 0, 0);
}

void generateHash(char* text,char* hash)
{
    unsigned char generatedHash[crypto_generichash_BYTES];
    crypto_generichash(generatedHash,sizeof(generatedHash),text,strlen(text),0,0);
    strcpy(hash,generatedHash);
}

void compressTxt(char *inputTxt, char *compressedTxt) 
{
	// zlib struct
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    // setup "inputTxt" as the input and "compressedTxt" as the compressed output
    defstream.avail_in = (uInt)strlen(inputTxt)+1; // size of input, string + terminator
    defstream.next_in = (Bytef *)inputTxt; // input char array
    defstream.avail_out = (uInt)sizeof(compressedTxt); // size of output
    defstream.next_out = (Bytef *)compressedTxt; // output char array
    
    // compression
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);
}