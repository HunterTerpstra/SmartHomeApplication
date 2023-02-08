//
// Created by Ryan Chan on 2022-11-07.
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

#ifndef INC_3307_BASENODE_H
#define INC_3307_BASENODE_H

class BaseNode {

public:

    struct NodeRecord {

        // participation weighting, previous value (to avoid recalculations)
        float node_val, node_weight;
        BaseNode* target;

    };

protected:

    int self_id;

    // input and output nodes
    std::vector<struct NodeRecord*> inputs_v;
    std::vector<struct NodeRecord*> outputs_v;

public:

    /**
     * @brief Initialises node contents
     * @param node_id Integer ID of node
     */
    BaseNode(int node_id);

    /**
     * @brief Destroys node, default constructor
     */
    ~BaseNode();

    /**
     * @brief Returns ID of node
     * @return Node ID
     */
    int GetID ();

    /**
     * @brief Adds node to siphon output from, changing its own value. Also updates target node's data.
     * @param target Pointer to target node
     * @param target_weight Percentage influence of target node's changes
     */
    void AddInputNode (BaseNode* target, float target_weight);

    /**
     * @brief Adds node to send output to when the node's value has changed. Also updates target node's data.
     * @param target Pointer to target node
     * @param self_weight Percentage influence of itself on target node
     */
    void AddOutputNode (BaseNode* target, float self_weight);

    /**
     * @brief Removes target node from input list
     * @param target Pointer to target node
     */
    void RemoveInputNode (BaseNode* target);

    /**
     * @brief Removes target node from output list
     * @param target Pointer to target node
     */
    void RemoveOutputNode (BaseNode* target);

    /**
     * @brief Get vector of all input nodes
     * @return Vector of all input nodes
     */
    std::vector<struct NodeRecord*>* GetInputNodes ();

    /**
     * @brief Get vector of all output nodes
     * @return Vector of all output nodes
     */
    std::vector<struct NodeRecord*>* GetOutputNodes ();

    /**
     * @brief Callable by other nodes and main system, when the value of node must change through cascading input values
     * @param source_node Pointer to source of change
     * @param value Updated value
     * @return Success indicator
     */
    virtual int Update (BaseNode* source_node, float value);

    /**
     * @brief Prints out all data, used for debugging
     */
    void Firehose ();

    /**
     * @brief Returns node type
     * @return Node type
     */
    virtual std::string GetNodeType ();
};


#endif //INC_3307_BASENODE_H
