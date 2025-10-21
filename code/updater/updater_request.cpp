#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "updater_request.h"
#include "../server/server_manager.h"

char* old_version = NULL;

const char* server_filename = "serv_updated.out";
const char* server_address = "http://93.175.1.127:8080";

const int request_start_offset = 125;

static int CreateFile(const char* filename);
static int LaunchFile();
static int VerifyFile();
static ServState Kill(Request* request);


static ServState Kill(Request* request)
{
    printf("GET KILL REQUEST");

    char* ans = "Server killed";
    write(request->client_socket, ans, strlen(ans));

    return SERV_HALTED;
}
ServState UpdateFile(Request* request)
{ 
    char* filename; 
    size_t filename_size;

    char* file;
    size_t file_size;

    filename = request->buffer + request_start_offset + 1;
    filename_size = strlen(filename);

    file_size = request->buffer_len - request_start_offset - filename_size - 2;
    file = request->buffer + request_start_offset + filename_size + 2;

    FILE* new_file = fopen(filename, "w+");

    fwrite(file, file_size, 1, new_file);

    fclose(new_file);
    
    return SERV_CORRECT;
}


ServState CommandParse(Request* request)
{
    if(request->buffer_len <= request_start_offset || !request->buffer) return SERV_BUFFER_INVALID;

    if(request->buffer[request_start_offset] == 'u') return UpdateFile(request);
    if(request->buffer[request_start_offset] == 'k') return Kill(request);

    return SERV_CORRECT;
}