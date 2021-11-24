#include "../include.h"

typedef struct{
    GSocket* socket;
    GSocketAddress* address;
    int clientId;
}clientParam;

void send_text(clientParam*);
void receive_text(clientParam*);
void receive_script(clientParam*);
int  checkHash(char*);
void extractStingHash(char*,char*,char*);
void test_hash();
//void extract_string_hash(char*, char*, char*, char*);
void decompress(int, char*,char *);
void test_compressed();
void extract_string_3(char*, char*, char* , char* );