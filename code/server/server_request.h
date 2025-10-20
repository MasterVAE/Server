#ifndef SERVER_REQUEST_H_
#define SERVER_REQUEST_H_

#include "server_manager.h"

ServErr Kill();
ServErr Test();
ServErr Request(const char* buffer, size_t buffer_len);

#endif // SERVER_REQUEST_H_