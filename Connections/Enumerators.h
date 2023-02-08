//
// Created by Ryan Chan on 2022-11-27.
//

#ifndef STRING
#define STRING

#include <string>
#include <cstring>
#include <string.h>

#endif

#ifndef INC_3307_ENUMERATORS_H
#define INC_3307_ENUMERATORS_H
#include <string>
struct Parameter {
    std::string name;
    std::string value;
};

enum HTTPMethod { GET, POST, PUT, PATCH, DELETE, OPTIONS, HTTP_METHOD_INVALID };
enum HTTPSection { REQUEST_SECTION, HEADER_SECTION, BODY_SECTION, HTTP_SECTION_INVALID };
enum RequestSection { REQUEST_METHOD, REQUEST_URI, REQUEST_PROTOCOL, REQUEST_SECTION_INVALID };
enum UriSection { URI_RESOURCE, URI_PARAMETER_KEY, URI_PARAMETER_VALUE, URI_SECTION_INVALID };
enum Action { ACTION_ADD, ACTION_REMOVE, ACTION_UPDATE, ACTION_GET, ACTION_SEND, ACTION_NONE };

#endif //INC_3307_ENUMERATORS_H
