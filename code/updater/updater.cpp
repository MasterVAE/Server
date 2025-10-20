#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "updater.h"

static int CreateFile();
static int LaunchFile();

int UpdateServer()
{

}

int CreateFile()
{
    
}

int LaunchFile() 
{
    printf("LAUNCHING NEW VERSION OF SERVER...\n");

    const char *command = "build/server.out biba &";
    system(command);

    return 0;
}



#define PORT 8000
#define BUFFER_SIZE 4096

static void handle_request(int client_socket, struct sockaddr_in *client_addr) {
    char buffer[BUFFER_SIZE] = {0};
    char client_ip[INET_ADDRSTRLEN];
    
    // Получаем IP клиента
    inet_ntop(AF_INET, &(client_addr->sin_addr), client_ip, INET_ADDRSTRLEN);
    
    int bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Request from %s:\n%s\n", client_ip, buffer);
    }
    
    const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n";
    
    write(client_socket, response, strlen(response));
    write(client_socket, client_ip, strlen(client_ip));
    
    close(client_socket);
}

int main() 
{
    printf("Updater server turning on...\n");

    int server_fd, client_socket;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int opt = 1;
    
    // Создаем сокет
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Настраиваем опции сокета
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // Настраиваем адрес
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Биндим сокет
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Слушаем подключения
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Updater server started on port %d\n", PORT);
    printf("Waiting for connections...\n\n");
    
    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }
        
        handle_request(client_socket, &client_addr);
    }
    
    close(server_fd);
    return 0;
}