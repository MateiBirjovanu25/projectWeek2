#include "../include.h"

typedef struct{
    GSocket* socket;
    GSocketAddress* addr;
    int clientID;
}clientParam;

void sendText(clientParam*);
void receiveText(clientParam*);
void sendScript(clientParam*);
void generateHash(char*,char*);
void compression(unsigned char*,const char*);
void compressTxt(char *, char *);