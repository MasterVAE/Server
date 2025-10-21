#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server_manager.h"

#define BUFFER_SIZE 4096

static ServState HandleRequest(int client_socket);
static ServState CommandParse(const char* buffer, size_t buffer_len, int socket);

static ServState HandleRequest(Server* serv, int client_socket) 
{
    char buffer[BUFFER_SIZE] = {0};
    
    const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n";
    
    write(client_socket, response, strlen(response));
    write(client_socket, "BIBA", strlen("BIBA"));

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
    
    printf("Server starting up on port %d\n", server->port);
    
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
