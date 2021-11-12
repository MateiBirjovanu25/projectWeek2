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
        if(g_socket_receive(clients[targetId].socket,text,100,0,0) == -1)
            printf("error while receiving\n");
        printf("received text from target\n");
        g_socket_send(aC->socket,text,100,0,0);
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