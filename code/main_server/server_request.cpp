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
    if(!request->buffer) return SERV_BUFFER_INVALID;

    printf("PARSING: %s\n", request->buffer);

    int i = 0;
    int enter = 0;
    for(i = 0; i < request->buffer_len; i++)
    {
     
        //fprintf(stderr, "[%d] %c\n",i, request->buffer[i]);
        if(request->buffer[i] == '\n') enter++;

        if(enter == 6) break;
    }

    const char* ans = "Instruction not found";
    write(request->client_socket, ans, strlen(ans));

    return SERV_CORRECT;
}