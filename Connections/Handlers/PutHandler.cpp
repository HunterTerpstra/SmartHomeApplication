/**
 * @file PutHandler.cpp
 * @author Shuhan Dong (you@domain.com)
 * @brief This class get all the nodes using the parser tree and store it onto the struct. 
 * Details:
 * There are two levels of the tree, first is the node itself, the name of the node and the threshold if the node. 
 * The second level includes input nodes and output nodes. For each of them, they have names and weights. 
 * @version 0.1
 * @date 2022-11-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
//
// Created by Ryan Chan on 2022-11-26.
//

#include "PutHandler.h"

std::string* PutHandler::handle_request (int http_method, std::string* http_uri, std::vector<struct Parameter*>* parameters,
                                         std::string* http_header, std::string* http_body) {
    auto* parser_obj = new Parser();
    pt::ptree* tree = parser_obj->getParseTree(*http_body);

    auto node = tree->get_child("Node");

    auto* addnode_payload = new NodePipe::PackageNode();


    addnode_payload->node_name = new std::string("no_name"); addnode_payload->value = -1.0f;
    addnode_payload->remaining_packages = 0; addnode_payload->action = ACTION_ADD;
    addnode_payload->input_nodes = new std::vector<std::string>(); addnode_payload->input_weights = new std::vector<float>();
    addnode_payload->output_nodes = new std::vector<std::string>(); addnode_payload->output_weights = new std::vector<float>();
    try {

        for (auto & i : tree->get_child("Node")) { // go through all children of XML tree
            if (std::strcmp(i.first.c_str(), "name") == 0) {
                delete (addnode_payload->node_name);
                addnode_payload->node_name = new std::string(i.second.data());
            } else if (std::strcmp(i.first.c_str(), "value") != 0)
                addnode_payload->value = std::stof(i.second.data());


                // if input node, take data
            else if (std::strcmp(i.first.c_str(), "input") == 0) {
                std::string *temp_name = nullptr;
                float temp_weight = -1.0f;
                for (auto &j: i.second) { // for all entries within
                    if (std::strcmp(j.first.c_str(), "name") == 0)
                        temp_name = new std::string(j.second.data());
                    if (std::strcmp(j.first.c_str(), "weight") == 0) {
                        temp_weight = std::stof(j.second.data());
                    }
                }
                if (temp_name != nullptr && temp_weight != -1.0f) {
                    addnode_payload->input_nodes->push_back(*temp_name);
                    addnode_payload->input_weights->push_back(temp_weight);
                }
            }

                // if output node, take data
            else if (std::strcmp(i.first.c_str(), "output") == 0) {
                std::string *temp_name = nullptr;
                float temp_weight = -1.0f;
                for (auto &j: i.second) { // for all entries within
                    if (std::strcmp(j.first.c_str(), "name") == 0)
                        temp_name = new std::string(j.second.data());
                    if (std::strcmp(j.first.c_str(), "weight") == 0)
                        temp_weight = std::stof(j.second.data());
                }
                if (temp_name != nullptr && temp_weight != -1.0f) {
                    addnode_payload->output_nodes->push_back(*temp_name);
                    addnode_payload->output_weights->push_back(temp_weight);
                }
            }
        }
        if (!addnode_payload->input_nodes->empty() && !addnode_payload->output_nodes->empty()) {
            NodePipe* pipe = NodePipe::instance(PIPE_ID);
            pipe->write(SYSTEM_ID, addnode_payload);
        } else {
            std::cerr << "Empty" << std::endl;
        }


    } catch (const boost::property_tree::ptree_error &e) {
        std::cerr << "Not all properties found" << std::endl;
    }

    std::string* response = new std::string("HTTP 200 OK");
    return (response);
}