#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "updater_request.h"
#include "../server/server_manager.h"

static ServState Kill(Request* request);
static ServState UpdateFile(Request* request);

static ServState Kill(Request* request)
{
    printf("GET KILL REQUEST\n");

    const char* ans = "Server halted";
    write(request->client_socket, ans, strlen(ans));

    return SERV_HALTED;
}
static ServState UpdateFile(Request* request)
{ 
    printf("GET UPDATE REQUEST\n");

    printf("Updating file -> %s\n", request->fields[request->current_field].value);
    const char* filename = "out.txt"; 

    char* file = request->fields[request->current_field + 1].value;
    size_t file_size = request->fields[request->current_field + 1].data_size;

    printf("FILENAME: %s\n", filename);
    printf("FILE_SIZE: %lu\n", file_size);

    FILE* new_file = fopen(filename, "w+");
    fwrite(file, file_size, 1, new_file);
    fclose(new_file);
    
    const char* ans = "File updated";
    write(request->client_socket, ans, strlen(ans));

    return SERV_CORRECT;
}

ServState CommandParse(Request* request)
{
    for(request->current_field = 0; request->current_field < request->field_count; request->current_field++)
    {
        Field* f = &request->fields[request->current_field];
        if(!strcmp(f->name, "command"))
        {
            if(!f->value) return SERV_CORRECT;

            if(!strcmp(f->value, "kill"))
            {
                return Kill(request);
            }
            if(!strcmp(f->value, "update"))
            {
                return UpdateFile(request);
            }
        }
    }

    const char* ans = "Instruction not found";
    write(request->client_socket, ans, strlen(ans));

    return SERV_CORRECT;
}