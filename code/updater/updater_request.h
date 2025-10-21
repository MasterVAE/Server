#ifndef UPDATER_REQUEST_H_
#define UPDATER_REQUEST_H_

#include "../server/server_manager.h"

ServState UpdateFile(Request* request);
ServState CommandParse(Request* request);

#endif // UPDATER_REQUEST_H_