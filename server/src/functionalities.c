#include "functionalities.h"


void receiveText(activeClient aC,GMutex mtx,char* text,int* done)
{
    // g_mutex_lock(&mtx);
    g_socket_send(aC.socket,"send",10,0,0);
    g_socket_receive(aC.socket,text,1024,0,0);
    // g_mutex_unlock(&mtx);

    // g_mutex_lock(&mtx);
    // *done = 1;

    // printf("signal sent\n");
    // g_mutex_unlock(&mtx);
}

void sendText(activeClient aC,GMutex mtx,char* text,int* done)
{
    // while(1)
    // {
    //     g_mutex_lock(&mtx);
    //     if(*done == 1)
    //     {
    //         g_mutex_unlock(&mtx);
    //         break;
    //     }
    //     g_mutex_unlock(&mtx);
    // }
    // printf("sending...\n");
    // g_mutex_lock(&mtx);
    g_socket_send(aC.socket,text,1024,0,0);
    // g_mutex_unlock(&mtx);
    // printf("sent\n");
    
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