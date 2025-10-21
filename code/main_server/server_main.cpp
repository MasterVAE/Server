#include <stdio.h>

#include "../server/server_manager.h"
#include "server_request.h"

char* password = NULL;

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
    serv.port = 8080;
    serv.RequestParser = CommandParse;

    ServState error = Host(&serv);
    
    if(error == SERV_CORRECT) return 0;

    return 1;
}