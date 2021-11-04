#include "functionalities.h"


void sendText(clientPararameter* parameter)
{
    g_mutex_lock(&(parameter->mtx));
    g_socket_receive(parameter->cl1,parameter->text,1024,0,0);
    g_mutex_unlock(&(parameter->mtx));

    g_mutex_lock(&(parameter->mtx));
    *(parameter->done) = 1;
    //g_cond_signal(&(parameter->cond));
    printf("signal sent\n");
    g_mutex_unlock(&(parameter->mtx));
}

void receiveText(clientPararameter* parameter)
{
    // g_mutex_lock(&(parameter->mtx));
    // printf("waiting...\n");
    // //while(*(parameter->done) == 0)
    // //g_cond_wait(&(parameter->cond),&(parameter->mtx));
    // g_mutex_unlock(&parameter->mtx);

    while(1)
    {
        g_mutex_lock(&(parameter->mtx));
        if(*(parameter->done) == 1)
        {
            g_mutex_unlock(&(parameter->mtx));
            break;
        }
        g_mutex_unlock(&(parameter->mtx));
    }


    printf("sending...\n");
    g_mutex_lock(&(parameter->mtx));
    g_socket_send(parameter->cl1,parameter->text,1024,0,0);
    g_mutex_unlock(&(parameter->mtx));
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