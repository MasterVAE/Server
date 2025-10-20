#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *command = "build/server.out biba &";

    printf("Attempting to launch another C program with sudo...\n");
    system(command);

    return 0;
}