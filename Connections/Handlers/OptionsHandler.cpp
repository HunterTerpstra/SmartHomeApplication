//
// Created by Ryan Chan on 2022-11-26.
//

#include "OptionsHandler.h"

std::string* OptionsHandler::handle_request (int http_method, std::string* http_uri, std::vector<struct Parameter*>* parameters,
                             std::string* http_header, std::string* http_body) {

    return (new std::string("HTTP/1.1 200 OK"));

}
