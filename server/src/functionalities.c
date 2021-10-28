#include "functionalities.h"
void sendRecvText(clientPar* clients)
{
    int clientType1,clientType2;
    g_socket_receive(clients->cl1,&clientType1,4,0,0);
    g_socket_receive(clients->cl2,&clientType2,4,0,0);

    if(clientType1==2)
    {
        GSocket* aux;
        aux = clients->cl1;
        clients->cl1 = clients->cl2;
        clients->cl2 = aux;
    }

    char request[100];
    g_socket_receive(clients->cl1,request,100,0,0);
    if(strcmp(request,"send") == 0)
    {}
    else if(strcmp(request,"receive") == 0)
    {
        GSocket* aux;
        aux = clients->cl1;
        clients->cl1 = clients->cl2;
        clients->cl2 = aux;
    }
    else
    {
        g_socket_send(clients->cl1,"invald input\n",100,0,0);
        g_socket_send(clients->cl2,"invald input\n",100,0,0);
        return ;
    }

    int clientType = 1; //sender
    g_socket_send(clients->cl1,&clientType,4,0,0);
    clientType = 2; //receiver
    g_socket_send(clients->cl2,&clientType,4,0,0);

    g_socket_send(clients->cl1,"start sending\n",100,0,0);
    g_socket_send(clients->cl2,"receveing...\n",100,0,0);

    char text[1024];
    g_socket_receive(clients->cl1,text,1024,0,0);

    g_socket_send(clients->cl2,text,1024,0,0);
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