#include "../include.h"
typedef struct 
{
    GSocket* cl1;
    GSocket* cl2;
}clientPar;

/*
input: clients:clientPar*, secondClient:GSocket*
ouput: -
receives text from one client and sends it to the other 
*/
void sendReceiveText(clientPar*,GSocket*);
void dbTest();