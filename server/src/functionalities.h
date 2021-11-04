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
}clientPararameter;

void sendText(clientPararameter*);
void receiveText(clientPararameter*);
void dbTest();