#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../server/server_manager.h"
#include "server_request.h"

ServState Kill(Request* request)
{
    printf("GET KILL REQUEST");

    char* ans = "Server killed";
    write(request->client_socket, ans, strlen(ans));

    return SERV_HALTED;
}

ServState Test(Request* request)
{
    printf("GET TEST REQUEST");

    char* ans = "OK";
    write(request->client_socket, ans, strlen(ans));

    return SERV_CORRECT;
}

ServState Ask(Request* request)
{
    printf("GET ASK REQUEST");

    char* ans = "BIBA";
    write(request->client_socket, ans, strlen(ans));

    return SERV_CORRECT;
}

ServState CommandParse(Request* request)
{
    if(request->buffer_len < 126 || !request->buffer) return SERV_BUFFER_INVALID;

    if(request->buffer[125] == 'k') return Kill(request);
    if(request->buffer[125] == 't') return Test(request);
    if(request->buffer[125] == 'a') return Ask (request);

    return SERV_CORRECT;
}