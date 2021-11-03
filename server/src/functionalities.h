#include "../include.h"
typedef struct 
{
    GSocket* cl1;
    GSocket* cl2;
    char command[100];
    GMutex mtx;
    GCond cond;
}clientPararameter;

/*
input: clients:clientPar*, secondClient:GSocket*
ouput: -
receives text from one client and sends it to the other 
*/
void sendText(clientPararameter*,char*,int*);
void receiveText(clientPararameter*,char*,int*);
void dbTest();