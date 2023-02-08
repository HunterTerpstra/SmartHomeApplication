//
// Created by Ryan Chan on 2022-11-26.
//

#include "Handler.h"

Handler::Handler() = default;

Handler::~Handler() = default;

std::string* Handler::handle_request (int http_method, std::string* http_uri, std::vector<struct Parameter*>* parameters,
                                     std::string* http_header, std::string* http_body) {

    std::cerr << "Handler " << http_method << " behaviour not defined." << std::endl;
    return (new std::string("HTTP/1.1 501 Not Implemented"));

}