#include "update_agent.h"

void update(char* file){

    printf("I am in the update agent function\n");
    FILE *fp;

    fp = fopen(file, "rx");
    char file2[15] = "misc/test.sh";
    char permission[50] = "chmod +x ";
    strcat(permission, file);
    printf("%s\n", permission);
    
    system(permission);
    printf("DONE PERM\n");
    system(file); //run the script file
    printf("DONE\n");
}