#include "serverActions.h"

void extractStringHash(char* text,char* hash)
{
    bzero(hash,1024);
    int indexHash = 0;
    int i=0;
    while(true)
    {
        if(text[i] == '!')
            break;
        hash[indexHash] = text[i];
        indexHash++;
        i++;
    }
}

void generateHash(char* text,char* hash)
{
    unsigned char generatedHash[crypto_hash_sha256_BYTES];
    crypto_hash_sha256(generatedHash,text,sizeof(text));
    strcpy(hash,generatedHash);
    printf("hash: \n");
    for(int i=0;i<32;i++)
    {
        printf("%c",hash[i]);
        if(hash[i] == '!')
        {
            hash[i] = '7';
            printf("! replaced\n");
        }
    }
    printf("\n");
    hash[32] = 0;
}

int checkHash(char* text,char* compressed)
{
    char hash[1024];
    extractStringHash(text,hash);
    char generatedHash[1024];
    generateHash(compressed,generatedHash);
    printf("recv hash: \n");
    printf("%s\n",hash);
    printf("gen hash: \n");
    printf("%s\n",generatedHash);
    if(strcmp(hash,generatedHash) == 0)
    {
        addToDatabase(compressed);
        return 0;
    }
    return 1;
}