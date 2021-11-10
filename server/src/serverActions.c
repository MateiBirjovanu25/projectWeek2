#include "serverActions.h"


void receiveText(activeClient* aC,int targetId)
{
    activeClient* clients = aC->activeClients;
    char text[1024];
    GMutex mtx = aC->mutexes[targetId];
    g_mutex_lock(&mtx); //lock the mutex in order to stom other threads from interfering with the transfer
        g_socket_send(&clients[targetId].socket,"send text",10,0,0);
        g_socket_receive(&clients[targetId].socket,text,1024,0,0);
    g_mutex_unlock(&mtx);
        g_socket_send(&aC->socket,text,1024,0,0);
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