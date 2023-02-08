//
// Created by Ryan Chan on 2022-11-03.
//

#ifndef INPUTLIB
#define INPUTLIB

#include <iostream>
#include <stdio.h>

#endif /* INPUTLIB */

#ifndef VECTOR
#define VECTOR

#include <vector>

#endif /* VECTOR */

#ifndef BASENODE_INCLUDE
#define BASENODE_INCLUDE

#include "BaseNode.h"

#endif /* BASENODE_INCLUDE */


#ifndef INC_3307_NODE_H
#define INC_3307_NODE_H

/**
 * @brief Node is used for internal nodes - input sensors, output devices, or intermediate nodes.
 */
class GraphNode : public BaseNode {
private:

    // accumulator for activation function
    float sum_f;

    // activation threshold (>=)
    float thresh_f;

public:

    /**
     * @param node_id Node ID
     * @param activation_threshold Threshold where a node is considered activated
     */
    GraphNode(int node_id, float activation_threshold);

    /**
     * @brief Callable function for other nodes to update this node's value
     * @param source_node Pointer to source node
     * @param value Updated value of source node
     * @return Success indicator
     */
    int Update (BaseNode* source_node, float value) override;

    /**
     * @return Node type
     */
    std::string GetNodeType () override;

    /**
     * @return Current internal value of node
     */
    float GetValue ();

};


#endif //INC_3307_NODE_H
