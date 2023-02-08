//
// Created by Ryan Chan on 2022-11-06.
//

#ifndef LOGGER_INCLUDE
#define LOGGER_INCLUDE
#include "logger.h"
#endif /* LOGGER_INCLUDE */

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif /* VECTOR */


#ifndef CENTRALNODE_INCLUDE
#define CENTRALNODE_INCLUDE
#include "Nodes/CentralNode.h"
#endif /* CENTRALNODE_INCLUDE */


#ifndef GRAPHNODE_INCLUDE
#define GRAPHNODE_INCLUDE
#include "Nodes/GraphNode.h"
#endif /* GRAPHNODE_INCLUDE */

#ifndef NODEPIPE_INCLUDE
#define NODEPIPE_INCLUDE
#include "Connections/NodePipe.h"
#endif /* NODEPIPE_INCLUDE */

#ifndef ENUMERATORS_INCLUDE
#define ENUMERATORS_INCLUDE

#include "Connections/Enumerators.h"

#endif /* ENUMERATORS_INCLUDE */

#ifndef INC_3307_CENTRAL_H
#define INC_3307_CENTRAL_H

#define SELF_PIPE_ID 0
#define ALIEN_PIPE_ID 1


class Central {
private:

    // node list for storing all in system
    std::vector<GraphNode*> node_list;
    std::vector<std::string*> node_names;
    std::vector<GraphNode*> input_list;

    int counter; // counter for node IDs
    CentralNode* central_node;

    const logger* _log;

    /**
     * @brief Adds a node to the graph, if it has not been created yet.
     * @param name Name of the node to be added
     * @param input_connections IDs of all nodes to take input values from
     * @param input_weightings Percentage multiplier of input nodes
     * @param output_connections IDs of all nodes this node updates its values to
     * @param output_weightings Percentage multiplier of the current node in the output nodes
     * @return
     */
    int AddNode (std::string* name, std::vector<int> input_connections, std::vector<float> input_weightings,
                 std::vector<int> output_connections, std::vector<float> output_weightings);



public:
    /**
     * @brief Non-returning constructor that runs central system. Handles all activity with nodes. Changes to nodes must be made through
     *  a pipe, rather than directly accessing data.
     */
    Central();

    /**
     * Unused destructor, as the central system should never exit unless explicitly terminated.
     */
    ~Central();

    /**
     * @brief Returns a particular node on the graph.
     * @param target The node ID, as an integer
     * @return Node queried
     */
    GraphNode* GetNode (int target);

    /**
     * @brief Removes node from a graph, and removes its associations to other nodes.
     * @param target Integer index of node to be removed
     */
    void RemoveNode (int target);

    /**
     * @brief Returns node acting as a centralized anchor point for all input and output nodes.
     * @return Node queried
     */
    CentralNode* GetCentralNode ();

    /**
     * @brief Prints out detailed debug information about all nodes. Used for debugging.
     */
    void Firehose ();
};

#endif //INC_3307_CENTRAL_H
