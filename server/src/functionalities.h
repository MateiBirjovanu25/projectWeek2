#include "../include.h"
typedef struct 
{
    GSocket* cl1;
    GSocket* cl2;
}clientPar;

void sendRecvText(clientPar* parameter);