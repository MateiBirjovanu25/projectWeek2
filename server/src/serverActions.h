#include "../include.h"
typedef struct
{
    GSocket *socket;
    int id;
    GMutex *mutexes;
    struct activeClient* activeClients;
    bool* working;
    int* checkArray;
}activeClient;

int parseText(char*);
void receiveText(activeClient*,int);
void dbTest();
void sodiumTest();