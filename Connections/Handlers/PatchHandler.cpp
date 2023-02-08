//
// Created by Ryan Chan on 2022-11-26.
//

#include "PatchHandler.h"

std::string* PatchHandler::handle_request (int http_method, std::string* http_uri, std::vector<struct Parameter*>* parameters,
                             std::string* http_header, std::string* http_body) {

    // create payload
    auto* payload = new NodePipe::PackageNode();
    payload->node_name = new std::string(http_uri->data() + 1, http_uri->data() + http_uri->size());
    payload->action = ACTION_UPDATE;

    payload->value = -1.0f;
    // see if any parameters are "value"
    for (auto & i : *parameters) {
        if (std::memcmp(i->name.data(), "value", 5) == 0) {
            payload->value = std::stof(i->value);
        }
    }

    if (payload->value == -1.0f) {
        delete(payload->node_name);
        delete(payload);
        return (new std::string("HTTP/1.1 400 Bad Request"));
    }

    // if there exists a "value" key, assign its corresponding value and send through pipe
    else {
        NodePipe* pipe = NodePipe::instance(PIPE_ID);
        pipe->write(SYSTEM_ID, payload);

        return (new std::string("HTTP/1.1 200 OK"));
    }

}