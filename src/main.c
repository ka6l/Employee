#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>

#include "files.h"

void usage(){

    printf("Hello world");
}

int main(int argc, char *argv[]){

    char *filepath = NULL;
    bool newFile = false;
    int c = 0;

    //U: Usage | F: Filepath | N: Is a new file? | 
    while((c = getopt(argc, argv, "f:n")) != -1){

        switch(c){

            case 'f':
                filepath = optarg;
                break;

            case 'n':
                newFile = true;
                printf("newFile is true");
            
            case '?':
                // printf("Unknown value");
                usage();

            default:
                return -1;

        }
    }

    // printf("%d", newFile);
    // printf("%s", filepath);

    if(filepath == NULL){
        printf("Enter filepath");
        return -1;
    }

    if(newFile){
        createFile(filepath);
    }



    return 0;
}