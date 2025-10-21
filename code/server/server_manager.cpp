#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <curl/curl.h>

#include "server_manager.h"

#define BUFFER_SIZE 4096

static ServState HandleRequest(int client_socket);
static ServState CommandParse(const char* buffer, size_t buffer_len, int socket);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

static ServState HandleRequest(Server* serv, int client_socket) 
{
    char buffer[BUFFER_SIZE] = {0};
    
    const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n";
    
    write(client_socket, response, strlen(response));

    ServState state = SERV_CORRECT;

    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read > 0) 
    {
        buffer[bytes_read] = '\0';
        Request request = {};
        request.buffer = buffer;
        request.buffer_len = bytes_read;
        request.client_socket = client_socket;
        state = serv->RequestParser(&request);
    }
    
    close(client_socket);

    return state;
}

ServState Host(Server* server) 
{
    printf("Server starting up on port %d\n", server->port);

    int server_fd, client_socket;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int opt = 1;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)     return SOCKET_FAILED;
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) return SETSOCKOPT_FAIL;
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server->port);
    
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
        if(error != SERV_CORRECT) break;
    }
    
    close(server_fd);

    return SERV_CORRECT;
}

struct MemoryStruct {
    char *memory;
    size_t size;
};


// Callback-функция для записи данных
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) 
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("Недостаточно памяти\n");
        return 0;
    }
    
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

char* MakeRequest(const char* url, const char* data)
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) 
        {
            fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
        }
        else if(chunk.size > 0)
        {
            printf("Response (%zu byte):\n%s\n", chunk.size, chunk.memory);
        }
        else
        {
            printf("Empty response\n");
        }
        
        curl_easy_cleanup(curl);
        free(chunk.memory);
    }
    
    curl_global_cleanup();
    return 0;
}

