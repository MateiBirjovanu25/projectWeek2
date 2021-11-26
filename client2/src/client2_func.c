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
    char originalBuff[1024], originalHash[256],compressedHash[256];
    char compressedBuff[1024];
    if(fd < 0)
    {
        printf("Wrong path or script doesn t exist!\n");
    }
    else
        read(fd, originalBuff, 1024);
    close(fd);

    GSocket* secondSocket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, NULL);
    if(g_socket_connect(secondSocket, cp->addr,0,0) == 0)
    {
        printf("Connecting error..\n");
        exit(1);
    }
    int clientType = 2;
    //generate compressed txt and hash, then concatenate original txt, compr hash and original hash.
    int n = strlen(originalBuff);
    char file_size[10];
    sprintf(file_size, "%d", n);
    printf("original buffer: %s\n",originalBuff);
    
    char replaceString[1024];
    strcpy(replaceString,originalBuff);


    generateHash(replaceString, originalHash);

    uLong ucompoSize = strlen(replaceString) + 1;
    uLong compSize = compressBound(ucompoSize);
    
    printf("original buffer: \n");
    printf("%s\n",replaceString);

    compress((Bytef*)compressedBuff,&compSize,(Bytef*)replaceString,ucompoSize);
    printf("size: %d\n",compSize);
    char buff[1024];  //endless cocatenation
    printf("compressed: \n");
    for(int i=0;i<compSize;i++)
    {
        printf("%c",compressedBuff[i]);
    }
    printf("\n");

    generateHash(compressedBuff, compressedHash);
    int index = 0;
    for(int i=0;i<strlen(compressedHash);i++)
    {
        buff[index] = compressedHash[i];
        index++;
    }
    buff[index] = '!';
    index++;
    for(int i=0;i<strlen(originalHash);i++)
    {
        buff[index] = originalHash[i];
        index++;
    }
    buff[index] = '!';
    index++;
    for(int i=0;i<strlen(file_size);i++)
    {
        buff[index] = file_size[i];
        index++;
    }

    printf("compressed hash\n");
    printf("%s\n",compressedHash);
    printf("uncompressed hash\n");
    printf("%s\n",originalHash);

    g_socket_send(secondSocket, &clientType, 4, 0, 0);
    g_socket_send(secondSocket, "passive", 100, 0, 0);
    g_socket_send(secondSocket, &cp->clientID, 4, 0, 0);
    g_socket_send(secondSocket,compressedBuff,1024,0,0);
    g_socket_send(secondSocket, buff, 1024, 0, 0);
    
}

void generateHash(char* text,char* hash)
{
    unsigned char generatedHash[crypto_hash_sha256_BYTES];
    bzero(hash,1024);
    printf("text: %s\n",text);
    crypto_hash_sha256(generatedHash,text,sizeof(text));
    strcpy(hash,generatedHash);
    printf("hash before: %s\n",hash);
    for(int i=0;i<32;i++)
        if(hash[i] == '!')
        {
            hash[i] = '7';
            printf("! replaced\n");
        }
    hash[32] = 0;
    printf("hash: %s\n",hash);
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