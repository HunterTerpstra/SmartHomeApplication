//
// Created by Ryan Chan on 2022-11-18.
//

#ifndef SOCKET
#define SOCKET

#include <sys/socket.h>

#endif //SOCKET

#ifndef INPUTLIB
#define INPUTLIB

#include <iostream>
#include <stdio.h>

#endif /* INPUTLIB */

#ifndef STRING
#define STRING

#include <string.h>
#include <cstring>
#include <string>

#endif /* STRING */

#ifndef UNISTD
#define UNISTD

#include <unistd.h>

#endif

#ifndef NETINET
#define NETINET

#include <netinet/in.h>
#include <arpa/inet.h>

#endif

#ifndef PTHREAD
#define PTHREAD

#include <pthread.h>

#endif /* PTHREAD */

#ifndef ERRNO
#define ERRNO

#include <errno.h>

#endif /* ERRNO*/

#ifndef VECTOR
#define VECTOR

#include <vector>

#endif /* VECTOR */

#ifndef QUEUE
#define QUEUE

#include <queue>

#endif /* QUEUE */

#ifndef TYPES
#define TYPES

#include <sys/types.h>

#endif /* TYPES */

#ifndef REGEX
#define REGEX

#include <regex>

#endif /* REGEX */

#ifndef NODEDROPBOX
#define NODEDROPBOX

#include "NodePipe.h"

#endif /* NODEDROPBOX */

#ifndef ENUMERATORS_INCLUDE
#define ENUMERATORS_INCLUDE

#include "Enumerators.h"

#endif /* ENUMERATORS_INCLUDE */

#ifndef HANDLERS
#define HANDLERS

#include "Handlers/GetHandler.h"
#include "Handlers/PostHandler.h"
#include "Handlers/PutHandler.h"
#include "Handlers/PatchHandler.h"
#include "Handlers/DeleteHandler.h"
#include "Handlers/OptionsHandler.h"

#endif /* HANDLERS */

#ifndef INC_3307_WIFI_H
#define INC_3307_WIFI_H

#define PORT 80
#define MAX_CONNECTIONS FD_SETSIZE

/**
 * HTTP server and access point for all attached resources.
 */
class Wifi {
protected:
    static int active_socket;

    /**
     * @brief Sends a 400 Bad Request response and shuts down connection
     */
    static void send_bad_response ();

    /**
     * @brief Sends 401 Unauthorized response if request should not be made from resource
     */
    static void send_not_allowed_response ();

    /**
     * @brief Takes an entire input string, parses, and calls the appropriate handler to address requests
     * @param request_text Entire HTTP request including the request itself, the header, and the body
     * @return Success indicator
     */
    static char process_request (std::string* request_text);

public:
    /**
     * @brief Non-exiting loop that constantly monitors a socket for incoming connections. Main foundation of server.
     */
    static void wifi_run();

};



#endif //INC_3307_WIFI_H
