#ifndef SERVER_MANAGER_H_
#define SERVER_MANAGER_H_

enum ServState
{
    SERV_CORRECT,
    SOCKET_FAILED,
    SETSOCKOPT_FAIL,
    BIND_FAIL,
    LISTEN_FAIL,
    SERV_COMMAND_NOT_FOUND,
    SERV_BUFFER_INVALID,
    SERV_HALTED
};

typedef struct Request
{
    char* buffer;
    size_t buffer_len;

    int client_socket;
} Request;

typedef struct Server
{
    int port;
    char* password;
    ServState (*RequestParser) (Request*);
} Server;

ServState Host(Server* server);
char* MakeRequest(const char* url, const char* data);

#endif // SERVER_MANAGER_H_