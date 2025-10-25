#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "../server/server_manager.h"

//26129
const char* weburl_upd = "ru.tuna.am:26129";

typedef struct
{
    int argc;
    char** argv;
} Args;

void MakeUpdateRequest   (Args* args);
void MakeDownloadRequest (Args* args);
void MakeKillRequest     (Args* args);
void MakeTestRequest     (Args* args);

Field* MakeFields(size_t count);
void FreeFields(Field* fields, size_t count);

int main(int argc, char* argv[]) 
{
    Args args = {argc, argv};
    if(args.argc <= 1)
    {
        printf("No argument found\n");
        return 0;
    }

    if(!strcmp(argv[1], "kill"))    MakeKillRequest(&args);  
    if(!strcmp(argv[1], "update"))  MakeUpdateRequest(&args);
    if(!strcmp(argv[1], "download"))MakeDownloadRequest(&args);
    if(!strcmp(argv[1], "test"))    MakeTestRequest(&args);
    
    return 0;
}

void MakeUpdateRequest(Args* args)
{
        if(args->argc <= 2)
    {
        printf("No argument found\n");
        return;
    }
    char* filename = args->argv[2];

    Request request = {};
    request.field_count = 3;
    request.fields = MakeFields(request.field_count);
    request.url = weburl_upd;

    request.fields[0].type = SYSTEM_FIELD;
    strcpy(request.fields[0].name, "argc");
    strcpy(request.fields[0].value, "2");

    request.fields[1].type = COMMAND_FIELD;
    strcpy(request.fields[1].name,"command");
    strcpy(request.fields[1].value, "update");

    request.fields[2].type = FILE_FIELD;
    strcpy(request.fields[2].name, "file");
    strcpy(request.fields[2].value, filename);

    printf("MAKING REQUEST:\n");

    MakeRequest(&request);

    FreeFields(request.fields, request.field_count);
}

void MakeDownloadRequest(Args* args)
{
    if(args->argc <= 2)
    {
        printf("No argument found\n");
        return;
    }
    char* filename = args->argv[2];

    Request request = {};
    request.field_count = 3;
    request.fields = MakeFields(request.field_count);
    request.url = weburl_upd;

    request.fields[0].type = SYSTEM_FIELD;
    strcpy(request.fields[0].name, "argc");
    strcpy(request.fields[0].value, "2");

    request.fields[1].type = COMMAND_FIELD;
    strcpy(request.fields[1].name,"command");
    strcpy(request.fields[1].value, "download");

    request.fields[2].type = FILE_FIELD;
    strcpy(request.fields[2].name, "file");
    strcpy(request.fields[2].value, filename);

    printf("MAKING REQUEST:\n");

    MakeRequest(&request);

    FreeFields(request.fields, request.field_count);
}

void MakeKillRequest(Args*)
{
    Request request = {};
    request.field_count = 2;
    request.fields = MakeFields(request.field_count);
    request.url = weburl_upd;

    request.fields[0].type = SYSTEM_FIELD;
    strcpy(request.fields[0].name, "argc");
    strcpy(request.fields[0].value, "1");

    request.fields[1].type = COMMAND_FIELD;
    strcpy(request.fields[1].name, "command");
    strcpy(request.fields[1].value, "kill");

    printf("MAKING REQUEST:\n");
    request.url = weburl_upd;
    MakeRequest(&request);

    FreeFields(request.fields, request.field_count);
}

void MakeTestRequest(Args*)
{
    Request request = {};
    request.field_count = 2;
    request.fields = MakeFields(request.field_count);
    request.url = weburl_upd;

    request.fields[0].type = SYSTEM_FIELD;
    strcpy(request.fields[0].name, "argc");
    strcpy(request.fields[0].value, "1");

    request.fields[1].type = COMMAND_FIELD;
    strcpy(request.fields[1].name, "command");
    strcpy(request.fields[1].value, "test");

    printf("MAKING REQUEST:\n");
    request.url = weburl_upd;
    MakeRequest(&request);

    FreeFields(request.fields, request.field_count);
}

Field* MakeFields(size_t count)
{   
    Field* fields = (Field*)calloc(count, sizeof(Field));
    for(size_t i = 0; i < count; i++)
    {
        fields[i].name = (char*)calloc(50, sizeof(char));
        fields[i].value = (char*)calloc(50, sizeof(char));
    }
    return fields;
}

void FreeFields(Field* fields, size_t count)
{
    for(size_t i = 0; i < count; i++)
    {
        free(fields[i].name);
        free(fields[i].value);
    }
    free(fields);
}