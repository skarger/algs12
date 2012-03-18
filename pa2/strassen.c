#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

#define MAX_COMMAND_LENGTH 1024

int main(int argc, char **argv) {
    if (argc != 4)
        error(1,"usage: strassen flag dimension inputfile","");

    int exit_status = 0;
    char cmd[MAX_COMMAND_LENGTH + 1];
    snprintf(cmd, MAX_COMMAND_LENGTH, 
            "java Strassen %s %s %s", argv[1], argv[2], argv[3]);
    exit_status = system(cmd);

    return exit_status;
}

