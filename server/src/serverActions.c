#include "serverActions.h"

int parseText(char* text)
{
    char id[10];
    int index = 0;
    bool found = false;
    int len = strlen(text);
    for(int i=0;i<len;i++)
    {
        if(text[i] == '!')
        {
            found = true;
            text[i] = 0;
            continue;
        }
        if(found == true)
        {
            id[index] = text[i];
            index++;
        }
    }
    int idInt = atoi(id);
    return idInt;
}


void extractStringHash(char* text,char* content,char* hash)
{
    int indexContent = 0;
    bzero(content,100);
    bzero(hash,100);
    int indexHash = 0;
    bool found = false;
    int len = strlen(text);
    for(int i=0;i<len;i++)
    {
        if(text[i] == '!' && found == true)
            break;

        if(text[i] == '!')
        {
            found = true;
            continue;
        }

        if(found != false)
        {
            content[indexContent] = text[i];
            indexContent++;
        }
        else
        {
            hash[indexHash] = text[i];
            indexHash++;
        }
    }
}

void generateHash(char* text,char* hash)
{
    unsigned char generatedHash[crypto_generichash_BYTES];
    crypto_generichash(generatedHash,sizeof(generatedHash),text,strlen(text),0,0);
    strcpy(hash,generatedHash);
}

void addToDatabase(char* compressedFile)
{
    MYSQL* con = mysql_init(NULL);
    if(mysql_real_connect(con,"localhost","matei","ietam",
                         "projectWeek2",0,NULL,0)==NULL)
                         {
                             printf("connect error\n");
                             exit(1);
                         }
    char query[256];
    sprintf(query,"insert into files values(NULL,'%s');",compressedFile);
    printf("%s\n",query);
    mysql_query(con,query);
}

int checkHash(char* text)
{
    char content[100];
    char hash[256];
    extractStringHash(text,content,hash);
    printf("content: %s",content);
    printf("hash: %s",hash);
    char generatedHash[256];
    generateHash(content,generatedHash);
    if(strcmp(hash,generatedHash) == 0)
    {
        addToDatabase(content);
        return 0;
    }
    return 1;
}

void receiveText(activeClient* aC,int targetId)
{
    activeClient* clients = aC->activeClients;
    char text[100];
    printf("target socket: %d\n",clients[targetId].socket);
    printf("receiver socket: %d\n",aC->socket);
    GMutex mtx = aC->mutexes[targetId];
        printf("sending\n");
        g_socket_send(clients[targetId].socket,"send text",100,0,0);
        printf("sent receive request to %d\n",targetId);

        while(clients[targetId].secondSocket == NULL); //waiting for the client to create the second socket
        printf("second socket in thread: %d\n",clients[targetId].secondSocket);

        g_socket_receive(clients[targetId].secondSocket,text,100,0,0);

        //g_socket_close(clients[targetId].secondSocket,0);

        clients[targetId].secondSocket = NULL;

        printf("received text from target\n");

        while(clients[aC->id].secondSocket == NULL); //waiting for the client to create the second socket

        printf("started sending\n");

        g_socket_send(clients[aC->id].secondSocket,text,100,0,0);

        clients[aC->id].secondSocket = NULL;
        printf("sent\n");
}

void receiveScript(activeClient* aC,int targetId)
{
    activeClient* clients = aC->activeClients;
    char text[100];
    printf("target socket: %d\n",clients[targetId].socket);
    printf("receiver socket: %d\n",aC->socket);
    
    printf("sending\n");
    g_socket_send(clients[targetId].socket,"send script",100,0,0);
    printf("sent receive request to %d\n",targetId);

    while(clients[targetId].secondSocket == NULL); //waiting for the target client to create the second socket
    printf("second socket in thread: %d\n",clients[targetId].secondSocket);

    g_socket_receive(clients[targetId].secondSocket,text,100,0,0);

    //g_socket_close(clients[targetId].secondSocket,0);

    printf("received text from target\n");

    if(checkHash(text) == 0) //check integrity of the file
        g_socket_send(clients[targetId].secondSocket,"success",100,0,0);
    else
        g_socket_send(clients[targetId].secondSocket,"fail",100,0,0);

    clients[targetId].secondSocket = NULL;

    while(clients[aC->id].secondSocket == NULL); //waiting for the receiving client to create the second socket

    printf("started sending\n");

    g_socket_send(clients[aC->id].secondSocket,text,100,0,0);

    clients[aC->id].secondSocket = NULL;
    printf("sent\n");
}

void dbTest()
{
    MYSQL* con = mysql_init(NULL);
    if(mysql_real_connect(con,"localhost","matei","ietam",
                         "projectWeek2",0,NULL,0)==NULL)
                         {
                             printf("error");
                             exit(1);
                         }
    if(mysql_query(con,"SELECT * FROM Animals"))
    {
        printf("error");
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        printf("error\n");
        exit(0);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }
}

void sodiumTest()
{
    unsigned char hash[crypto_generichash_BYTES];
    crypto_generichash(hash,sizeof(hash),"caine",6,0,0);
    printf("%s\n",hash);
    crypto_generichash(hash,sizeof(hash),"cainl",6,0,0);
    printf("%s\n",hash);
}