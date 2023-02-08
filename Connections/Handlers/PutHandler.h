//
// Created by Ryan Chan on 2022-11-26.
//

#ifndef STRING
#define STRING

#include <string>
#include <cstring>
#include <string.h>

#endif

#ifndef VECTOR
#define VECTOR

#include <vector>

#endif /* VECTOR */

#ifndef PARSER_INCLUDE
#define PARSER_INCLUDE

#include "../../Parser/Parser.h"

#endif /* PARSER_INCLUDE */

#ifndef HANDLER_INCLUDE
#define HANDLER_INCLUDE

#include "Handler.h"

#endif

#ifndef INC_3307_PUTHANDLER_H
#define INC_3307_PUTHANDLER_H


class PutHandler : public Handler{


public:

    /**
     * @brief Specifically handles Put requests (adding nodes)
     * @author Bella Dong
     * @param http_method Enumerator for type of HTTP request
     * @param http_uri URI, stripped of parameters
     * @param parameters Tuple of value+key pairs
     * @param http_header Header contents
     * @param http_body Body contents, should be in XML format
     * @return
     */
    std::string* handle_request (int http_method, std::string* http_uri, std::vector<struct Parameter*>* parameters,
                                         std::string* http_header, std::string* http_body) override;

};

#endif //INC_3307_PUTHANDLER_H
