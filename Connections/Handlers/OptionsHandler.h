//
// Created by Ryan Chan on 2022-11-26.
//

#ifndef HANDLER_INCLUDE
#define HANDLER_INCLUDE

#include "Handler.h"

#endif

#ifndef INC_3307_OPTIONSHANDLER_H
#define INC_3307_OPTIONSHANDLER_H


class OptionsHandler : public Handler {

public:

    /**
     * @brief Specifically handles Options requests. Returns a list of available options for API calls.
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


#endif //INC_3307_OPTIONSHANDLER_H
