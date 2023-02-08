
#include "Wifi.h"

int Wifi::active_socket = 0;

void Wifi::send_bad_response () {

    char response_bad[] = "HTTP/1.1 400 Bad Request\n\nBad request format\n";
    send(active_socket, response_bad, strlen(response_bad), 0);
    shutdown(active_socket, SHUT_RDWR);

}

void Wifi::send_not_allowed_response () {

    char response_bad[] = "HTTP/1.1 400 Bad Request\n\nBad request format\n";
    send(active_socket, response_bad, strlen(response_bad), 0);
    shutdown(active_socket, SHUT_RDWR);

}

char Wifi::process_request (std::string* request_text) {

    char response_ok[] = "HTTP/1.1 200 OK\n\n"
                         "<!DOCTYPE html>\n"
                         "<html>\n"
                         "\t<body>\n"
                         "\t\t<h1>Test Page</h1>\n"
                         "\t</body>\n"
                         "</html>\n";

    // sep newline
    int http_method = HTTP_METHOD_INVALID;
    std::string* http_uri = nullptr;
    auto* parameters = new std::vector<struct Parameter*>();
    std::string* http_header = nullptr;
    std::string* http_body = nullptr;

    int content_stage = REQUEST_SECTION; // store stage of content currently
    int header_stage = REQUEST_METHOD;
    int uri_stage = URI_RESOURCE;

    auto* temp_param = new struct Parameter;

    // loop through all characters of request
    // start of parse loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    for (char* tok_start = const_cast<char*>(request_text->data()),
            *tok_end = const_cast<char*>(request_text->data() + request_text->size()),
            *curr = tok_start; curr <= tok_end; curr++) {
        switch (content_stage) {
            case (REQUEST_SECTION):
                // can progress onto next stage under either of 2 conditions:
                // space: delimiter marking next section of either method, uri, or protocol
                // newline: delimiter marking exit of request line, only if all 3 sections above have been fulfilled
                if (header_stage == REQUEST_METHOD) {
                    // if we are currently expecting the HTTP method and encounter a space, marking end of token
                    if (*curr == ' ') {
                        std::string temp_method_str = std::string(tok_start, curr - 1);
                        if (std::memcmp(temp_method_str.data(), "GET", temp_method_str.size()) == 0)
                            http_method = GET;
                        else if (std::memcmp(temp_method_str.data(), "POST", temp_method_str.size()) == 0)
                            http_method = POST;
                        else if (std::memcmp(temp_method_str.data(), "PUT", temp_method_str.size()) == 0)
                            http_method = PUT;
                        else if (std::memcmp(temp_method_str.data(), "PATCH", temp_method_str.size()) == 0)
                            http_method = PATCH;
                        else if (std::memcmp(temp_method_str.data(), "DELETE", temp_method_str.size()) == 0)
                            http_method = DELETE;
                        else if (std::memcmp(temp_method_str.data(), "OPTIONS", temp_method_str.size()) == 0)
                            http_method = OPTIONS;
                        else {
                            send_bad_response();
                            http_method = HTTP_METHOD_INVALID;
                        }

                        header_stage = REQUEST_URI; // move onto next stage of header

                        if (curr + 1 >= tok_end) { // if the very first delimiter is already at the end, invalid request
                            send_bad_response();
                            return (1);
                        } // otherwise, set token start pointer forwards to character after delimiter
                        else tok_start = curr + 1;
                    }
                }

                else if (header_stage == REQUEST_URI) {
                    if (*curr == '?' && uri_stage == URI_RESOURCE) { // parameter delimiter found, marks end of uri, start of parameters
                        http_uri = new std::string(tok_start, curr);
                        uri_stage = URI_PARAMETER_KEY; // move onto parameter key portion
                        tok_start = curr + 1;
                    }
                    else if (*curr == ' ') { // header delimiter found
                        if (uri_stage == URI_RESOURCE) { // targeting the directory portion
                            http_uri = new std::string(tok_start, curr);
                            delete(temp_param); // since URI section was completed without parameters, delete preallocated
                        }
                        else if (uri_stage == URI_PARAMETER_KEY) { // on parameter key stage
                            // delete structure as parameter is incomplete
                            delete(temp_param);
                        } else { // on parameter value stage
                            // complete structure and add to vector
                            temp_param->value = std::string(tok_start, curr);
                            parameters->push_back(temp_param);
                            temp_param = new struct Parameter;
                        }
                        tok_start = curr + 1;
                        header_stage = REQUEST_PROTOCOL; // move onto protocol for next loop
                    }
                    else if (*curr == '=' && uri_stage == URI_PARAMETER_KEY) { // transition from key to value
                        temp_param->name = std::string(tok_start, curr);
                        tok_start = curr + 1; // set token start pointer forwards to character after delimiter
                        uri_stage = URI_PARAMETER_VALUE; // process next parameter
                    }
                    else if (*curr == '&' && uri_stage == URI_PARAMETER_VALUE) { // transition from value to next parameter
                        temp_param->value = std::string(tok_start, curr);
                        temp_param = new struct Parameter;
                        tok_start = curr + 1; // set token start pointer forwards to character after delimiter
                        uri_stage = URI_PARAMETER_KEY; // process next parameter
                    }
                    else { // nothing happens! not a special character or delimiter

                    }
                }

                else { // process HTTP/1.1 line
                    // if newline, move onto header contents - http protocol (typically HTTP/1.1) is not stored.
                    if (*curr == '\n') {
                        content_stage = HEADER_SECTION;
                        tok_start = curr + 1;
                    }
                }
                break;
            case (HEADER_SECTION): // header section, read until final section
                // if final section, record as header and set front pointer
                if ((curr != tok_end && (*(curr-1) == '\n' && *curr == '\n')) || curr == tok_end) { // if empty line or end of file
                    if (tok_start >= (curr - 1))
                        http_header = nullptr;
                    else
                        http_header = new std::string(tok_start, curr - 1);
                    tok_start = curr + 1;
                    content_stage = BODY_SECTION;
                }
                break;
            case (BODY_SECTION):

                http_body = new std::string(tok_start, tok_end);
                curr = tok_end + 1;
                break;
            default:
                std::cerr << "Bad things happen to bad people" << std::endl;
                break;
        }
    }
    // end of parse loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // redirect to request handler
    switch (http_method) {
        case (GET): {
            // create handler, write to string, send string through connection and close
            auto get_handler = GetHandler();
            std::string* temp = get_handler.handle_request(http_method, http_uri, parameters, http_header, http_body);
            std::cout << "Result: " << temp->c_str() << std::endl;
            std::cout << "Result length: " << std::to_string(temp->size()) << std::endl;
            send(active_socket,
                 temp->c_str(),
                 temp->size(), 0);
            shutdown(active_socket, SHUT_RDWR);
            break;
        }

        case (POST): {
            auto post_handler = PostHandler();
            std::string* temp = post_handler.handle_request(http_method, http_uri, parameters, http_header, http_body);
            send(active_socket,
                 temp->c_str(),
                 temp->size(), 0);
            shutdown(active_socket, SHUT_RDWR);
            delete(temp);
            break;
        }
        case (PUT): {
            auto put_handler = PutHandler();
            std::string* temp = put_handler.handle_request(http_method, http_uri, parameters, http_header, http_body);
            send(active_socket,
                 temp->c_str(),
                 temp->size(), 0);
            shutdown(active_socket, SHUT_RDWR);
            delete(temp);
            break;
        }
        case (PATCH): {
            auto patch_handler = PatchHandler();
            std::string* temp = patch_handler.handle_request(http_method, http_uri, parameters, http_header, http_body);
            send(active_socket,
                 temp->c_str(),
                 temp->size(), 0);
            shutdown(active_socket, SHUT_RDWR);
            delete(temp);
            break;
        }
        case (DELETE): {
            auto delete_handler = DeleteHandler();
            std::string* temp = delete_handler.handle_request(http_method, http_uri, parameters, http_header, http_body);
            send(active_socket,
                 temp->c_str(),
                 temp->size(), 0);
            shutdown(active_socket, SHUT_RDWR);
            delete(temp);
            break;
        }
        case (OPTIONS): {
            auto options_handler = OptionsHandler();
            options_handler.handle_request(http_method, http_uri, parameters, http_header, http_body);
            break;
        }
        default: {
            auto undefined_handler = Handler();
            std::string* temp = undefined_handler.handle_request(http_method, http_uri, parameters, http_header, http_body);
            send(active_socket,
                 temp->c_str(),
                 temp->size(), 0);
            shutdown(active_socket, SHUT_RDWR);
            delete(temp);
            break;
        }
    }

    //send(active_socket, response_ok, strlen(response_ok), 0);
    shutdown(active_socket, SHUT_RDWR);
    return (0);
}

void Wifi::wifi_run() {

    int main_socket = socket(AF_INET, SOCK_STREAM, 0);

    int client_socket[MAX_CONNECTIONS];
    for (int & i : client_socket) {
        i = 0;
    }

    auto* fds = (fd_set*) malloc(sizeof(fd_set));

    int option = 1;
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));

    auto* addr = (struct sockaddr_in*) malloc (sizeof(struct sockaddr_in));
    addr->sin_port = htons(PORT);
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;

    // bind to particular socket on OS
    if (bind(main_socket, reinterpret_cast<const sockaddr*>(addr), sizeof(struct sockaddr_in)) < 0) {
        std::cerr << "Bind could not be executed." << std::endl;
        exit(EXIT_FAILURE);
    }

    // maximum queue backlog of 8
    if (listen(main_socket, 8) < 0) {
        std::cerr << "Listen failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    while (1) {

        FD_ZERO(fds); // clears out all sets from socket array
        FD_SET(main_socket, fds); // set main socket as active in bit field

        int max_socket = main_socket;

        // get all active client connections
        for (int temp_sock : client_socket) {

            // copy temporarily
            // if current socket is active, add to socket set
            if (temp_sock > 0)
                FD_SET(temp_sock, fds);

            // store the highest priority active socket
            if (temp_sock > max_socket)
                max_socket = temp_sock;

        }

        // max_socket = highest active socket, or main socket if none available
        // for system to take on more sockets, all previous connections must be finished

        // selects highest priority connection from above and if nothing, blocks until main socket queued
        if (select(max_socket + 1, fds, nullptr, nullptr, nullptr) < 0 && errno != EINTR) {
            std::cerr << "Program blocked for read unsuccessfully." << std::endl;
            exit(EXIT_FAILURE);
        }

        // if previously no socket was connected, and main socket now has unblocked (new connection established)
        if (FD_ISSET(main_socket, fds)) {

            // accept incoming socket and assign to secondary socket
            auto addrlen = (socklen_t)sizeof(struct sockaddr);
            active_socket = accept(main_socket, (struct sockaddr *) &addr, &addrlen);

            // curr_sock is diverted secondary socket
            // if curr_sock is invalid socket
            if (active_socket < 0) {
                // if connection not accepted
                std::cerr << "Incoming connection not accepted." << std::endl;
                exit(EXIT_FAILURE);
            }

                // if curr_sock is valid socket, read data
            else {

                char data_buf[4096];
                recv(active_socket, data_buf, 1024, 0);
                //std::cout << "Incoming message: " <<  in_buf->c_str() << std::endl;
                auto* in_buf = new std::string(data_buf);
                process_request(in_buf);
            }

            // add socket to end of list
            int i = 0;
            while (client_socket[i++] != 0 && i < MAX_CONNECTIONS);
            client_socket[i] = active_socket;

        }

        // check for messages
        for (int & i : client_socket) {

            // evaluate every socket
            int descriptor = i;

            // if socket exists in set (established previously)
            if (FD_ISSET(descriptor, fds)) {

                char in_buf[1024];

                // if socket was closed in latest action
                if (read(descriptor, in_buf, 1024) == 0) {
                    i = 0;
                }

                    // if something was simply sent
                else {
                    std::cerr << "Message received." << std::endl;
                    char response[] = "HTTP/1.1 400 Bad Request\n\n"
                                      "You should not be seeing this.\n";
                    send(descriptor, response, strlen(response), 0);
                    shutdown(descriptor, SHUT_RDWR);
                }

            }

            else {
                // socket is not used
            }
        }
    }
}