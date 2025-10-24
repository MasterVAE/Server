#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../server/server_manager.h"

//26129
const char* weburl_upd = "ru.tuna.am:23719";
const char* weburl_srv = "https://9620312a90221f110c04c6fd8bc90fab.serveo.net";

const char* file_start_name = "text.txt";
const char* file_end_name = "text2.txt";

char* InitializeBuffer(size_t* size, FILE* input_file);

int main(int argc, char* argv[]) 
{
    if(!strcmp(argv[1], "k"))
    {
        Request request1 = {};
        request1.field_count = 2;
        request1.fields = (Field*)calloc(request1.field_count, sizeof(Field));

        request1.fields[0].type = SYSTEM_FIELD;
        request1.fields[0].name = "argc";
        request1.fields[0].value = "1";

        request1.fields[1].type = COMMAND_FIELD;
        request1.fields[1].name = "command";
        request1.fields[1].value = "kill";

        printf("MAKING REQUEST:\n");
        //printf("%d:\n", request1.fields[1].type);
        request1.url = weburl_upd;
        MakeRequest(&request1);

        request1.url = weburl_srv;
        MakeRequest(&request1);

        free(request1.fields);
    }    
    if(!strcmp(argv[1], "u"))
    {
        char* filename = argv[2];

        Request request1 = {};
        request1.field_count = 3;
        request1.fields = (Field*)calloc(request1.field_count, sizeof(Field));
        request1.url = weburl_upd;

        request1.fields[0].type = SYSTEM_FIELD;
        request1.fields[0].name = "argc";
        request1.fields[0].value = "2";

        request1.fields[1].type = COMMAND_FIELD;
        request1.fields[1].name = "command";
        request1.fields[1].value = "update";

        request1.fields[2].type = FILE_FIELD;
        request1.fields[2].name = "file";
        request1.fields[2].value = filename;

        printf("MAKING REQUEST:\n");

        MakeRequest(&request1);

        free(request1.fields);
    } 
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