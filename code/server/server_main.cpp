#include <stdio.h>

#include "server_manager.h"

char* password = NULL;

int main(int argc, char* argv[])
{
    if(argc > 1) password = argv[1];

    if(!password)
    {
        fprintf(stderr, "INCORRECT PASSWORD, TRY AGAIN...");
        return 1;
    }

    printf("PASSWORD: %s\n", password);

    Host();

    return 0;
}