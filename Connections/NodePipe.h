//
// Created by Ryan Chan on 2022-11-25.
//

#ifndef INPUTLIB
#define INPUTLIB

#include <iostream>
#include <stdio.h>

#endif /* INPUTLIB */

#ifndef STRING
#define STRING

#include <string.h>
#include <cstring>
#include <string>

#endif /* STRING */

#ifndef VECTOR
#define VECTOR

#include <vector>

#endif /* VECTOR */

#ifndef QUEUE
#define QUEUE

#include <queue>

#endif /* QUEUE */

#ifndef MUTEX
#define MUTEX

#include <mutex>

#endif /* MUTEX */

#ifndef INC_3307_NODEPIPE_H
#define INC_3307_NODEPIPE_H

/**
 * @brief Multi-target pipe for thread-safe small talk between components. Must use PackageNode.
 * Must use fixed targeting index:
 * 0 - Central system
 * 1 - Wifi
 * 2 - Bluetooth (not implemented)
 */
class NodePipe {
public:
    struct PackageNode {
        std::string* node_name;
        float value;
        int remaining_packages;
        int action;

        std::vector<std::string>* input_nodes;
        std::vector<float>* input_weights;

        std::vector<std::string>* output_nodes;
        std::vector<float>* output_weights;
    };

    /**
     * Singleton instantiator to create and return nodepipe object
     * @param self_index Index of pipe to create, belonging to caller object
     * @return Pipe pointer to singleton object
     */
    static NodePipe* instance(int self_index);

private:

    struct ProcessPipe {
        std::queue<struct PackageNode*>* in_pipe;
    };

    std::vector<struct ProcessPipe*> queues;

    static char is_created;
    static NodePipe* self;

    /**
     * @brief Initialises internal variables
     */
    NodePipe();
    ~NodePipe();

    /**
     * @brief Registers process and creates its pipe, if not already created
     * @param index Destination index for object's pipe
     */
    void add_process(int index);

public:

    /**
     * @brief Writes data to pipe
     * @param target_index Target pipe index for write
     * @param content Payload to send
     */
    void write (int target_index, struct PackageNode* content);

    /**
     * @brief Reads data from pipe, removing it when done
     * @param target_index Source pipe index for read
     * @return Payload read from pipe
     */
    struct PackageNode* read (int target_index);

    /**
     * @brief Reads data from pipe, leaving it when done
     * @param target_index Source pipe index for read
     * @return Payload read from pipe
     */
    struct NodePipe::PackageNode* peek (int target_index);

};


#endif //INC_3307_NODEDPIPE_H
