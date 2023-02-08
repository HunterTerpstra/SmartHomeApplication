//
// Created by Ryan Chan on 2022-11-26.
//

#ifndef STRING
#define STRING

#include <string>
#include <cstring>
#include <string.h>

#endif

#ifndef INPUTLIB
#define INPUTLIB

#include <iostream>
#include <stdio.h>

#endif /* INPUTLIB */

#ifndef ENUMERATORS_INCLUDE
#define ENUMERATORS_INCLUDE

#include "../Enumerators.h"

#endif /* ENUMERATORS_INCLUDE */

#ifndef NODEPIPE_INCLUDE
#define NODEPIPE_INCLUDE

#include "../NodePipe.h"

#endif /* NODEPIPE_INCLUDE */

#ifndef LOGGER_INCLUDE
#define LOGGER_INCLUDE
#include "../../logger.h"
#endif /* LOGGER_INCLUDE */

#ifndef INC_3307_HANDLER_H
#define INC_3307_HANDLER_H

#define PIPE_ID 1
#define SYSTEM_ID 0

class Handler {

private:



public:

    Handler();
    ~Handler();

    // separation of handlers enforces interface segregation, each behaviour is separated into their own inherited class
    // also allows for safe modification of individual HTTP methods without tearing up the entire system

    /***
     * @brief Handles incoming HTTP request after parsing
     * @author Ryan Chan
     * @param http_method Enumerator for type of HTTP request
     * @param http_uri URI, stripped of parameters
     * @param parameters Tuple of value+key pairs
     * @param http_header Header contents
     * @param http_body Body contents, should be in XML format
     * @return
     */
    virtual std::string* handle_request (int http_method, std::string* http_uri, std::vector<struct Parameter*>* parameters,
            std::string* http_header, std::string* http_body);

};


#endif //INC_3307_HANDLER_H
