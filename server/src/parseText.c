#include "serverActions.h"

int parseText(char* text)
{
    char id[10];
    int index = 0;
    bool found = false;
    int len = strlen(text);
    for(int i=0;i<len;i++)
    {
        if(text[i] == '!')
        {
            found = true;
            text[i] = 0;
            continue;
        }
        if(found == true)
        {
            id[index] = text[i];
            index++;
        }
    }
    int idInt = atoi(id);
    return idInt;
}