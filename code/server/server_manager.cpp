#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <assert.h>
#include <ctype.h>

#include "server_manager.h"

#define BUFFER_SIZE 1000000

static ServState HandleRequest(Server* serv, int client_socket);

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
static char* SkipLines(char* string, size_t lines);
static void RequestBufferParse(Request* request);
static void RequestBufferFree(Request* request);

static ServState HandleRequest(Server* serv, int client_socket) 
{
    char buffer[BUFFER_SIZE] = {0};
    
    const char *base_response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n";
    
    write(client_socket, base_response, strlen(base_response));

    ServState state = SERV_CORRECT;

    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read > 0) 
    {
        buffer[bytes_read] = '\0';

        Request request = {};
        request.buffer = buffer;
        request.buffer_len = (size_t)bytes_read;
        request.client_socket = client_socket;
        state = serv->RequestParser(&request);

        RequestBufferParse(&request);
        state = serv->RequestParser(&request);
        RequestBufferFree(&request);
    }
    
    close(client_socket);

    return state;
}

ServState Host(Server* server) 
{
    printf("Server trying to start on port %d\n", server->port);

    int server_fd, client_socket;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int opt = 1;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)     return SOCKET_FAILED;
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) return SETSOCKOPT_FAIL;
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((uint16_t)server->port);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)   return BIND_FAIL;
    
    if (listen(server_fd, 10) < 0)  return LISTEN_FAIL;
    
    printf("Server started up on port %d\n", server->port);
    
    while (1)
    {
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (client_socket < 0) 
        {
            fprintf(stderr, "Accept");
            continue;
        }
        ServState error = HandleRequest(server, client_socket);
        if(error != SERV_CORRECT) 
        {
            printf("SERVER STOPPED WITH ERRCODE: %d\n", error);
            break;
        }
    }
    
    close(server_fd);

    return SERV_CORRECT;
}

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void *user_param) 
{
    
    size_t realsize = size * nmemb;

    Request* request = (Request*)user_param;
    
    if(request->buffer) free(request->buffer);
    request->buffer = (char*)calloc(realsize + 1, sizeof(char));
    memcpy(request->buffer, contents, realsize);

    request->buffer_len = realsize;
    request->buffer[request->buffer_len] = '\0';
    
    return realsize;
}

char* MakeRequest(Request* request)
{
    CURL *curl;
    CURLcode res;
    
    Request response = {0};
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) 
    {  
        struct curl_mime *form = NULL;
        struct curl_mimepart *field = NULL;

        form = curl_mime_init(curl);

        for(size_t i = 0; i < request->field_count; i++)
        {
            field = curl_mime_addpart(form);
            curl_mime_name(field, request->fields[i].name);
            if(request->fields[i].type == FILE_FIELD)
            {
                FILE* test_file = fopen(request->fields[i].value, "rb");
                if(!test_file) 
                { 
                    fprintf(stdout, "NO FILE\n");
                    exit(1);
                }
                fseek(test_file, 0, SEEK_END);
                long file_size = ftell(test_file);
                fseek(test_file, 0, SEEK_SET);

                char str_file_size[50];
                sprintf(str_file_size, "%ld", file_size);

                curl_mime_data(field, str_file_size, CURL_ZERO_TERMINATED);

                printf("filesize: %ld \n", file_size);
                
                field = curl_mime_addpart(form);
                curl_mime_name(field, request->fields[i].name);
                curl_mime_filedata(field, request->fields[i].value);
            }
            else
            {
                curl_mime_data(field, request->fields[i].value, CURL_ZERO_TERMINATED);
            }
        }

        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L);
        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L); // 100KB buffer
        curl_easy_setopt(curl, CURLOPT_UPLOAD_BUFFERSIZE, 102400L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

        curl_easy_setopt(curl, CURLOPT_URL, request->url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) 
        {
            fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
        }
        else if(response.buffer_len > 0)
        {
            printf("Request (%zu byte):\n%s\n", response.buffer_len, response.buffer);
        }
        else
        {
            printf("Empty response\n");
        }

        free(response.buffer);

        curl_mime_free(form);
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    return 0; // Почему возвращаем 0? Должен быть char*?
}

static size_t my_strlen(const char* string)
{
    size_t len = 0;

    char c;
    while(isalnum(c = string[len++]) || c == '/' || c == '.');

    return len - 1;
}

static char* SkipLines(char* string, size_t lines)
{
    char* next = string;
    for(size_t i = 0; i < lines; i++)
    {
        next = strchr(next, '\n') + 1;
    }

    return next;
}

//FIXME проверка что прилетел мим
static void RequestBufferParse(Request* request)
{
    //printf("\n\nPARSING:\n%s\n\n\n", request->buffer);
    
    char* current_buffer = SkipLines(request->buffer, 7);
    current_buffer += 38;

    size_t len = my_strlen(current_buffer);
    current_buffer[len] = '\0';

    fprintf(stderr, "TYPE RECIEVED: %s\n", current_buffer);
    if(strncmp(current_buffer, "argc", len)) return;
        
    current_buffer += len + 1;
    current_buffer = SkipLines(current_buffer, 2);

    len = my_strlen(current_buffer);
    current_buffer[len] = '\0';

    size_t argc = 0;
    if(sscanf(current_buffer, "%lu", &argc) <= 0) return;

    current_buffer += len + 1;

    printf("ARGC: %lu\n", argc);

    request->field_count = argc;
    request->fields = (Field*)calloc(argc, sizeof(Field));

    for(size_t i = 0; i < argc; i++)
    {
        current_buffer = SkipLines(current_buffer, 2);
        //printf("\n\nPARSING:\n%s\n\n\n", current_buffer);
        current_buffer += 38;

        len = my_strlen(current_buffer);
        current_buffer[len] = '\0';

        request->fields[i].name = (char*)calloc(len + 1, sizeof(char));
        strcpy(request->fields[i].name, current_buffer);

        fprintf(stderr, "TYPE RECIEVED: %s\n", current_buffer);
        if(!strcmp(current_buffer, "file"))
        {
            current_buffer += len + 1;
            current_buffer = SkipLines(current_buffer, 2);

            size_t file_size = 0;
            sscanf(current_buffer, "%lu", &file_size);
            printf("filesize: %lu\n", file_size);
            request->fields[i].data_size = file_size;

            current_buffer = SkipLines(current_buffer, 2);
            current_buffer += 55;

            len = my_strlen(current_buffer);
            current_buffer[len] = '\0';

            request->fields[i].name = (char*)calloc(len + 1, sizeof(char));

            strcpy(request->fields[i].name, current_buffer);
            current_buffer += len + 1;
            current_buffer = SkipLines(current_buffer, 3);

            char* file_data = (char*)calloc(file_size, sizeof(char));
            memcpy(file_data, current_buffer, file_size);
            request->fields[i].value = file_data;
        
            return;
        }

        current_buffer += len + 1;
        current_buffer = SkipLines(current_buffer, 2);

        len = my_strlen(current_buffer);
        current_buffer[len] = '\0';

        request->fields[i].value = (char*)calloc(len + 1, sizeof(char));
        strcpy(request->fields[i].value, current_buffer);

        fprintf(stderr, "VALUE RECIEVED: %s\n", current_buffer);

        current_buffer += len + 1;
    }
}

static void RequestBufferFree(Request* request)
{
    for(size_t i = 0; i < request->field_count; i++)
    {
        free(request->fields[i].name);
        free(request->fields[i].value);
    }
    free(request->fields);
}