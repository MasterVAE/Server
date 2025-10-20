#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server_manager.h"
#include "server_request.h"

#define PORT 8080
#define BUFFER_SIZE 4096

static ServErr HandleRequest(int client_socket);
static int CommandParse(const char* buffer, size_t buffer_len);

static ServErr HandleRequest(int client_socket) 
{
    char buffer[BUFFER_SIZE] = {0};
    
    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read > 0) 
    {
        buffer[bytes_read] = '\0';
        CommandParse(buffer, (size_t)bytes_read);
    }
    
    const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n";
    
    write(client_socket, response, strlen(response));

    write(client_socket, "BIBA", strlen("BIBA"));
    
    close(client_socket);
}

ServErr Host() 
{
    int server_fd, client_socket;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int opt = 1;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)     return SOCKET_FAILED;
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) return SETSOCKOPT_FAIL;
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)   return BIND_FAIL;
    
    if (listen(server_fd, 10) < 0)  return LISTEN_FAIL;
    
    printf("Server started on port %d\n", PORT);
    printf("Server is listening on all network interfaces\n");
    printf("Waiting for connections...\n\n");
    
    while (1)
    {
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (client_socket < 0) 
        {
            fprintf(stderr, "Accept");
            continue;
        }
        
        ServErr error = HandleRequest(client_socket);
    }
    
    close(server_fd);

    return SERV_CORRECT;
}

static ServErr CommandParse(const char* buffer, size_t buffer_len)
{
    if(buffer_len == 0 || !buffer) return 1;

    if(buffer[0] == 'k') return Kill();
    if(buffer[0] == 't') return Test();
    if(buffer[0] == 'r') return Request(buffer, buffer_len);

    return SERV_COMMAND_NOT_FOUND;
}