#include "../include.h"
typedef struct
{
    GSocket *socket;
    GSocket *secondSocket;
    int id;
    GMutex *mutexes;
    struct activeClient* activeClients;
    bool* working;
    int* checkArray;
}activeClient;


typedef struct
{
    int* checkArray;
    GThread* threads;
    activeClient* clients;
    int* clientId;
}watchDogParam;

int checkHash(char*);
void addToDatabase(char* compressedFile);
int parseText(char*);
void receiveText(activeClient*,int);
void receiveScript(activeClient*,int);
void dbTest();
void sodiumTest();