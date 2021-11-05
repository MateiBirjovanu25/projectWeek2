#include "../include.h"
typedef struct
{
    GSocket* socket;
    int id;
    GMutex* mutexes;
    struct activeClient* activeClients;
}activeClient;

void receiveText(activeClient*,int);
void dbTest();
void sodiumTest();