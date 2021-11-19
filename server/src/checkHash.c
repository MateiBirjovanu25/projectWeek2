#include "serverActions.h"

void extractStringHash(char* text,char* content,char* hash)
{
    int indexContent = 0;
    bzero(content,100);
    bzero(hash,100);
    int indexHash = 0;
    bool found = false;
    int len = strlen(text);
    for(int i=0;i<len;i++)
    {
        if(text[i] == '!' && found == true)
            break;

        if(text[i] == '!')
        {
            found = true;
            continue;
        }

        if(found != false)
        {
            content[indexContent] = text[i];
            indexContent++;
        }
        else
        {
            hash[indexHash] = text[i];
            indexHash++;
        }
    }
}

void generateHash(char* text,char* hash)
{
    unsigned char generatedHash[crypto_generichash_BYTES];
    crypto_generichash(generatedHash,sizeof(generatedHash),text,strlen(text),0,0);
    strcpy(hash,generatedHash);
}

int checkHash(char* text)
{
    char content[100];
    char hash[256];
    extractStringHash(text,content,hash);
    printf("content: %s",content);
    printf("hash: %s",hash);
    char generatedHash[256];
    generateHash(content,generatedHash);
    if(strcmp(hash,generatedHash) == 0)
    {
        addToDatabase(content);
        return 0;
    }
    return 1;
}