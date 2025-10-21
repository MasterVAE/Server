#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../server/server_manager.h"

const char* weburl_upd = "http://93.175.1.127:8000";
const char* weburl_srv = "http://93.175.1.127:8080";

const char* file_start_name = "text.txt";
const char* file_end_name = "text2.txt";

struct MemoryStruct {
    char *memory;
    size_t size;
};


int main(int argc, char* argv[]) 
{
    MakeRequest(weburl_srv, argv[1]);
    MakeRequest(weburl_upd, argv[1]);      
    return 0;
}

static size_t FileLen(FILE* file);

char* InitializeBuffer(size_t* size, FILE* input_file)
{
    *size = FileLen(input_file);

    char* buff = (char*)calloc(*size + 3, sizeof(char));
    size_t len = fread(buff, sizeof(char), *size, input_file);

    buff[len] = '\0';
    buff[len+1] = '\0';

    *size = len;

    return buff;
}

static size_t FileLen(FILE* file)
{
    struct stat file_info;
    fstat(fileno(file), &file_info);
    return (size_t)file_info.st_size;
}