#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#define MAX_COMMAND_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3)
        error(1,"usage: kk inputfile <num items>","");

    int exit_status = 0;
    char cmd[MAX_COMMAND_LENGTH + 1];
    if (argc == 3) {
        snprintf(cmd, MAX_COMMAND_LENGTH, 
            "java -Xmx2048m KarmarkarKarp %s %s", argv[1], argv[2]);
    } else {
        snprintf(cmd, MAX_COMMAND_LENGTH, 
            "java -Xmx2048m KarmarkarKarp %s", argv[1]);
    }

    exit_status = system(cmd);

    return exit_status;
}

