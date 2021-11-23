#include "update_agent.h"

void update(char* file){

    printf("I am in the update agent function\n");

    char file2[15] = "misc/test.sh";
    char permission[50] = "chmod +x ";
    strcat(permission, file2);
    printf("%s\n", permission);
    
    system(permission);
    printf("DONE PERM\n");
    system(file2); //run the script file
    printf("DONE\n");
}