//
// Created by Ryan Chan on 2022-11-26.
//

#include "DeleteHandler.h"

std::string* DeleteHandler::handle_request (int http_method, std::string* http_uri, std::vector<struct Parameter*>* parameters,
                             std::string* http_header, std::string* http_body) {

    // if URI only contains /
    if (http_uri->size() == 1)
        return (new std::string("HTTP/1.1 400 Bad Request"));

    // if something exists in the URI
    else {

        // construct payload for pipe
        struct NodePipe::PackageNode* payload_out = new struct NodePipe::PackageNode();
        payload_out->remaining_packages = 0; payload_out->value = 0.0f;
        payload_out->action = ACTION_REMOVE;
        payload_out->node_name = new std::string(http_uri->data() + 1, http_uri->data() + http_uri->size());

        payload_out->input_nodes = nullptr; payload_out->input_weights = nullptr;
        payload_out->output_nodes = nullptr; payload_out->output_weights = nullptr;

        // instantiate singleton
        NodePipe* pipe_ptr = NodePipe::instance(PIPE_ID);
        // Logger message here: Multi-threaded pipe initialised in DELETE request
        logger::instance().write("DeleteHandler","Multi-threaded pipe initialised in DELETE request");

        // send to pipe
        pipe_ptr->write(SYSTEM_ID, payload_out);
        // Logger message here: DeleteHandler sent request to delete "payload_out->node_name"
        logger::instance().write("DeleteHandler","sent request to delete \"payload_out->node_name\"");

        // return ok result
        return (new std::string("HTTP/1.1 OK"));

    }

}