#include "Central.h"


Central::Central () : counter(0) {

    node_list = std::vector<GraphNode*>();
    node_names = std::vector<std::string*>();
    input_list = std::vector<GraphNode*>();

    central_node = new CentralNode();
    // create nodes for testing
    std::vector<int> input_connections = std::vector<int>(); std::vector<float> input_weightings = std::vector<float>();
    std::vector<int> output_connections = std::vector<int>(); std::vector<float> output_weightings = std::vector<float>();

    input_connections.push_back(-1); input_weightings.push_back(1.0f);

    AddNode(new std::string("Node_one"), input_connections, input_weightings, output_connections, output_weightings);

    input_connections = std::vector<int>(); input_weightings = std::vector<float>();
    output_connections = std::vector<int>(); output_weightings = std::vector<float>();

    input_connections.push_back(0); input_weightings.push_back(0.5f); input_connections.push_back(-1); input_weightings.push_back(0.5f);
    output_connections.push_back(-1); output_weightings.push_back(-1.0f);

    AddNode(new std::string("Node_two"), input_connections, input_weightings, output_connections, output_weightings);

    NodePipe* comms_pipe = NodePipe::instance(SELF_PIPE_ID);
    // Logger message here: Multi-threaded pipe initialised in central system
    logger::instance().write("Central","Multi-threaded pipe initialised in central system");

    std::cout << "Central system started." << std::endl;
    // Logger message here: Central system initialised - entering main loop
    logger::instance().write("Central","Central system initialised - entering main loop");

    while (1) {

        // hold until food comes through the pipe
        while(comms_pipe->peek(SELF_PIPE_ID) == nullptr);
        std::cout << "Request received by central system" << std::endl;

        // dinner has arrived
        int remaining_packets = 1;
        while (remaining_packets != 0) {

            struct NodePipe::PackageNode* payload = comms_pipe->read(SELF_PIPE_ID);
            remaining_packets = payload->remaining_packages;

            // if get all nodes
            if (payload->action == ACTION_GET && std::memcmp(payload->node_name->data(), "all", 3) == 0) {

                if (counter == 0) {
                    struct NodePipe::PackageNode* payload_out = new struct NodePipe::PackageNode();
                    payload_out->action = ACTION_NONE;
                    payload_out->node_name = new std::string("none");
                    payload_out->value = 1.0f; payload_out->remaining_packages = 0;

                    payload_out->input_nodes = nullptr; payload_out->input_weights = nullptr;
                    payload_out->output_nodes = nullptr; payload_out->output_weights = nullptr;

                    comms_pipe->write(ALIEN_PIPE_ID, payload_out);
                }

                // wake up programmer, we have nodes to send
                else {

                    for (int i = 0; i < counter; i++) {

                        GraphNode* curr = node_list.at(i);
                        struct NodePipe::PackageNode* payload_out = new struct NodePipe::PackageNode();

                        payload_out->action = ACTION_NONE;
                        payload_out->node_name = node_names.at(i);
                        payload_out->value = curr->GetValue(); payload_out->remaining_packages = counter - 1 - i;

                        payload_out->input_nodes = new std::vector<std::string>(); payload_out->input_weights = new std::vector<float>();
                        payload_out->output_nodes = new std::vector<std::string>(); payload_out->output_weights = new std::vector<float>();

                        // get node names and weights of all input nodes to node in question
                        for (auto & j : *curr->GetInputNodes()) {
                            if (j->target->GetID() == -1)
                                payload_out->input_nodes->push_back(std::string("central_node"));
                            else
                                payload_out->input_nodes->push_back(*node_names.at(j->target->GetID()));
                            payload_out->input_weights->push_back(j->node_weight);
                        }

                        // get node names and weights of all output nodes in question
                        for (auto & j : *curr->GetOutputNodes()) {
                            if (j->target->GetID() == -1)
                                payload_out->output_nodes->push_back(std::string("central_node"));
                            else {
                                payload_out->output_nodes->push_back(*node_names.at(j->target->GetID()));
                            }
                            payload_out->output_weights->push_back(j->node_weight);
                        }

                        comms_pipe->write(ALIEN_PIPE_ID, payload_out);
                    }
                }
            }

            // delete node????!!!111!??//
            else if (payload->action == ACTION_REMOVE) {

                int victim_index = -1;
                for (int i = 0; i < node_names.size(); i++) {
                    // if name matches
                    if (node_names.at(i) != nullptr && node_names.at(i)->size() == payload->node_name->size() &&
                    std::memcmp(node_names.at(i)->data(), payload->node_name->data(), payload->node_name->size()) == 0)
                        victim_index = i;
                }

                if (victim_index == -1) {
                    // Logger message here: Node payload->node_name removal was attempted, but node did not exist
                    logger::instance().write("Central","Node payload->node_name removal was attempted, but node did not exist");
                }

                // remove node
                else
                    RemoveNode(victim_index);

                delete(payload->node_name);
                delete(payload);
            }

            else if (payload->action == ACTION_UPDATE) {
                std::cerr << "Updating node " << payload->node_name->c_str() << " to " << std::to_string(payload->value) << std::endl;
                for (int i = 0; i < node_names.size(); i++) {
                    if (node_names.at(i) != nullptr && node_names.at(i)->size() == payload->node_name->size() &&
                        std::memcmp(node_names.at(i)->data(), payload->node_name->data(),
                                    payload->node_name->size()) == 0) {
                        node_list.at(i)->Update(central_node, payload->value);
                        std::cout << "Value of node " << payload->node_name->c_str() << " updated to " << std::to_string(payload->value) << std::endl;

                    }
                }

                delete(payload->node_name);
                delete(payload);
            }

            else if (payload->action == ACTION_ADD && payload->input_nodes->size() == payload->input_weights->size() &&
                                                      payload->output_nodes->size() == payload->output_weights->size()) {
                    std::cerr << "adding" << std::endl;
                    std::vector<int> input_temp = std::vector<int>();
                    std::vector<int> output_temp = std::vector<int>();
                    std::vector<float> input_temp_weightings = std::vector<float>();
                    std::vector<float> output_temp_weightings = std::vector<float>();

                    // for all input nodes
                    for (int i = 0; i < payload->input_nodes->size(); i++) {
                        // find correspondingly named node
                        for (auto & name : node_names) {
                            // if name matches, add node ID and weightings
                            if (std::memcmp(payload->node_name, name, name->size()) == 0) {
                                input_temp_weightings.push_back(payload->input_weights->at(i));
                                input_temp.push_back(i);
                            }
                            else if (std::strcmp(payload->node_name->data(), "central_node") == 0) {
                                input_temp_weightings.push_back(payload->input_weights->at(i));
                                input_temp.push_back(-1);
                            }
                        }
                    }

                    // for all output nodes
                    for (int i = 0; i < payload->output_nodes->size(); i++) {
                        for (auto & name : node_names) {
                            if (std::memcmp(payload->node_name, name, name->size()) == 0) {
                                output_temp_weightings.push_back(payload->output_weights->at(i));
                                output_temp.push_back(i);
                            }

                            else if (std::strcmp(payload->node_name->data(), "central_node") == 0) {
                                output_temp_weightings.push_back(payload->output_weights->at(i));
                                output_temp.push_back(-1);
                            }
                        }
                    }

                    AddNode(payload->node_name, input_temp, input_temp_weightings,
                            output_temp, output_temp_weightings);
                    Firehose();
            }

            else {

            }
        }

        //std::cout << "Central system finished parsing request" << std::endl;

    }

}


Central::~Central() = default;

// add node to graph and set upstream/downstream connections
int Central::AddNode (std::string* name, std::vector<int> input_connections, std::vector<float> input_weightings,
                      std::vector<int> output_connections, std::vector<float> output_weightings) {

    // create graph node and add to system list with same id as its position in system list
    GraphNode* temp_node = new GraphNode(counter, 1.0f);
    // insert to vector at position of its own id
    node_list.insert(node_list.begin() + counter, temp_node);

    /*if (queues.size() - 1 < target_index)
        queues.resize(target_index + 1);*/

    node_names.insert(node_names.begin() + counter, name);

    counter++;
    std::cerr << "Created node " << temp_node->GetID() << " and " << node_names.at(counter - 1)->c_str() << std::endl;
    // add all input connections to node
    for (int i = 0; i < input_connections.size(); i++) {
        // if node to add does not yet exist
        int input_connection = input_connections.at(i);
        if ((node_list.size() - 1 < input_connection ||
        (node_list.size() - 1 >= input_connection && node_list.at(input_connection) == nullptr)) && input_connection != -1) {
            printf("Target upstream node %d does not exist. Skipped.\n", input_connection);
        }

        // if node exists, add its reference to current node
        else {

            // add direct node reference to node under review
            // if -1, node is central system
            if (input_connection < 0) {
                // input socket is directly from central system

                central_node->AddOutputNode(temp_node, 1.0f);

                // add to list of inputs nodes in central system

                input_list.insert(input_list.begin() + temp_node->GetID(), temp_node);


            }

            else {

                temp_node->AddInputNode(node_list.at(input_connection), input_weightings.at(i));

            }
        }
    }

    // add all downstream connections to node
    for (int i = 0; i < output_connections.size(); i++) {

        // if node to add does not yet exist
        if (node_list.size() >= output_connections.at(i) && node_list.at(output_connections.at(i)) == nullptr && output_connections.at(i) != -1)
            std::cout << "Target upstream node does not exist. Skipped." << std::endl;
        // if node exists, add its reference to current node
        else {
            // add direct node reference to node under review
            if (output_connections.at(i) == -1) {
                // output socket directly leads to central system

                central_node->AddInputNode(temp_node, 1.0f);

            }
            else temp_node->AddOutputNode(node_list.at(output_connections.at(i)), 1.0f);
        }
    }
    // Logger message here: Node temp_node->GetID() added
    logger::instance().write("Central","Node temp_node->GetID() added");
    printf("GraphNode %d successfully created.\n", temp_node->GetID());
    return (temp_node->GetID());
}

GraphNode* Central::GetNode (int target) {
    if (node_list.at(target) != nullptr) {
        return (node_list.at(target));
    } else return (nullptr);
}

void Central::RemoveNode (int target) {

    // removal not implemented, but request is accepted

}

CentralNode* Central::GetCentralNode () {
    return (central_node);
}

void Central::Firehose () {
    printf("Central node:\n");
    central_node->Firehose();
}