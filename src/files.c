#include <fcntl.h>
#include <stdio.h>

void openFile(char *filepath){

}

int createFile(int argc, char *argv){

    int fd = open(&argv[1], O_RDONLY);
    
    if(fd != -1){
        printf("File exists already...");
        return fd;
    }

    fd = open(&argv[1], O_RDWR | O_CREAT, 0644);

    

    return fd;

}

