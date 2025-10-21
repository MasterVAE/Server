#include <stdio.h>
#include <stdlib.h>

#include "updater_request.h"
#include "../server/server_manager.h"

char* old_version = NULL;

const char* server_filename = "serv_updated.out";
const char* server_address = "https:bjsklfmsdklgnsdlgnsdgn";

static int CreateFile(const char* filename);
static int LaunchFile();
static int VerifyFile();
static int Kill();

int UpdateServer()
{
    CreateFile(server_filename);
    Kill();
    LaunchFile();
    if(VerifyFile())
    {

    }
}

static int Kill()
{

}

static int VerifyFile()
{
    
}

static int CreateFile(const char* filename)
{
    FILE* file = fopen(filename, "w+");



    fclose(file);
}

int LaunchFile() 
{
    printf("LAUNCHING NEW VERSION OF SERVER...\n");

    const char *command = "build/server.out biba &";
    system(command);

    return 0;
}

ServState CommandParse(Request* request)
{
    return SERV_CORRECT;
}