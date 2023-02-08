//
// Created by Ryan Chan on 2022-11-26.
//

#ifndef HANDLER_INCLUDE
#define HANDLER_INCLUDE

#include "Handler.h"

#endif

#ifndef LOGGER_INCLUDE
#define LOGGER_INCLUDE
#include "../../logger.h"
#endif /* LOGGER_INCLUDE */

#ifndef INC_3307_GETHANDLER_H
#define INC_3307_GETHANDLER_H


class GetHandler : public Handler {

public:

    /**
     * @brief Specifically handles Get requests (retrieve node information from the system)
     * @author Ryan Chan
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


#endif //INC_3307_GETHANDLER_H
