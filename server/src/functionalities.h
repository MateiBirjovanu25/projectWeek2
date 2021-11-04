#include "../include.h"
typedef struct 
{
    GSocket* cl1;
    GSocket* cl2;
    char command[100];
    GMutex mtx;
    GCond cond;
    char* text;
    int* done;
    int* check;
}clientPararameter;

/*
input: clients:clientPar*, secondClient:GSocket*
ouput: -
receives text from one client and sends it to the other 
*/
void sendText(clientPararameter*);
void receiveText(clientPararameter*);
void dbTest();