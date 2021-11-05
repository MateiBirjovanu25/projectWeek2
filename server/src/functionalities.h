#include "../include.h"
typedef struct
{
    GSocket* socket;
    int id;
    GMutex* mutexes;
    activeClient* activeClients;
}activeClient;

void receiveText(activeClient,GMutex,char*,int*);
void sendText(activeClient,GMutex,char*,int*);
void dbTest();