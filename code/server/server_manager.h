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

enum FieldType
{
    SYSTEM_FIELD,
    COMMAND_FIELD,
    FILE_FIELD
};
typedef struct Field
{
    FieldType type;
    char* name;
    char* value;
    size_t data_size;
} Field;

typedef struct Request
{
    const char* url;
    int client_socket;

    size_t field_count;
    Field* fields;

    char* buffer;
    size_t buffer_len;

    size_t current_field;
} Request;

typedef struct Server
{
    int port;
    char* password;
    ServState (*RequestParser) (Request*);
} Server;

ServState Host(Server* server);
char* MakeRequest(Request* request);

#endif // SERVER_MANAGER_H_