//
// Created by Ryan Chan on 2022-11-26.
//

#include "GetHandler.h"

// HTTP GET /device_name?type=name
// HTTP GET /device_id?type=id
// HTTP GET /all

std::string* GetHandler::handle_request (int http_method, std::string* http_uri, std::vector<struct Parameter*>* parameters,
                             std::string* http_header, std::string* http_body) {

    // if GET /all request
    // Logger message here: Handling GET request
    if (std::memcmp(http_uri->data() + 1, "all", 3) == 0) {
        auto* result = new std::string();
        *result += "HTTP/1.1 200 OK\n\n";
        *result += "<?xml version=\"1.0\"?>\n";
        *result += "<Nodes>";

        // instantiate singleton
        NodePipe* pipe_ptr = NodePipe::instance(PIPE_ID);
        // Logger message here: Multi-threaded pipe initialised in GET request
        logger::instance().write("GetHandler","Multi-threaded pipe initialised in GET request");

        // create request for all nodes, send to central system
        struct NodePipe::PackageNode* request_payload = new NodePipe::PackageNode();
        request_payload->node_name = new std::string("all"); request_payload->value = 0.0f;
        request_payload->remaining_packages = 0; request_payload->action = ACTION_GET;
        request_payload->input_nodes = nullptr; request_payload->input_weights = nullptr;
        request_payload->output_nodes = nullptr; request_payload->output_weights = nullptr;

        pipe_ptr->write(SYSTEM_ID, request_payload); // send request, currently empty


        // hold until something is available to read
        int remaining_payloads = 1;
        while (remaining_payloads != 0) {

            // hold until chicken nuggets are available
            while (pipe_ptr->peek(PIPE_ID) == nullptr);

            // something has been read
            struct NodePipe::PackageNode* payload = pipe_ptr->read(PIPE_ID);
            remaining_payloads = payload->remaining_packages;

            if (std::strcmp(payload->node_name->data(), "none") == 0) {
                *result += "</Nodes>";
                return (result);
            }

            *result += "<Node>";
            *result += "<name>"; *result += *payload->node_name; *result += "</name>";
            *result += "<value>"; *result += std::to_string(payload->value); *result += "</value>";

            for (int i = 0; i < payload->input_nodes->size(); i++) {
                *result += "<input>";
                *result += "<name>"; *result += payload->input_nodes->at(i); *result += "</name>";
                *result += "<weight>"; *result += std::to_string(payload->input_weights->at(i)); *result += "</weight>";
                *result += "</input>";
            }

            for (int i = 0; i < payload->output_nodes->size(); i++) {
                *result += "<output>";
                *result += "<name>"; *result += payload->output_nodes->at(i); *result += "</name>";
                *result += "<weight>"; *result += std::to_string(payload->output_weights->at(i)); *result += "</weight>";
                *result += "</output>";
            }

            *result += "</Node>";

            delete(payload);
        }

        *result += "</Nodes>";

        delete(request_payload);

        return (result);

    }

    // GET 127.0.0.1/ HTTP/1.1
    else {
        // Logger message here: Invalid GET request
        logger::instance().write("GetHandler","Invalid GET request");
        std::string* result = new std::string();
        *result += "Bad request";
        return (result);
    }
}
