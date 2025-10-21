#ifndef SERVER_REQUEST_H_
#define SERVER_REQUEST_H_

#include <stdlib.h>

#include "../server/server_manager.h"

ServState Kill(Request* request);
ServState Test(Request* request);
ServState Ask (Request* request);

ServState CommandParse(Request* request);

#endif // SERVER_REQUEST_H_