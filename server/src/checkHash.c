#include "serverActions.h"

void extractStringHash(char* text,char* content,char* hash)
{
    int indexContent = 0;
    bzero(content,1024);
    bzero(hash,1024);
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

        if(found == false)
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
    char content[1024];
    char hash[1024];
    extractStringHash(text,content,hash);
    printf("content: %s\n",content);
    printf("hash: %s\n",hash);
    char generatedHash[1024];
    generateHash(content,generatedHash);
    if(strcmp(hash,generatedHash) == 0)
    {
        addToDatabase(content);
        return 0;
    }
    return 1;
}