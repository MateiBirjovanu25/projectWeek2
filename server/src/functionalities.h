#include "../include.h"
typedef struct
{
    GSocket* socket;
    int id;
    GMutex* mutexes;
    activeClient* activeClients;
}activeClient;

void receiveText(activeClient*,int);
void dbTest();