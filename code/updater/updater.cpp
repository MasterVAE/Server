#include <stdio.h>
#include <stdlib.h>

#include "../server/server_manager.h"
#include "updater_request.h"

char* password;

int main(int argc, char* argv[])
{
    if(argc > 1) password = argv[1];

    if(!password)
    {
        fprintf(stderr, "INCORRECT PASSWORD, TRY AGAIN...");
        return 1;
    }

    Server serv = {};
    serv.password = password;
    serv.port = 8000;
    serv.RequestParser = CommandParse;

    ServState error = Host(&serv);
    if(error == SERV_CORRECT) return 0;

    printf("Error: %d\n", error);
    return 1;
}