#include "serverActions.h"

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