//
// Created by Ryan Chan on 2022-11-03.
//

#include "GraphNode.h"

GraphNode::GraphNode(int node_id, float activation_threshold) : BaseNode(node_id), sum_f(0.0f), thresh_f(activation_threshold) {

}

// process updated input value
int GraphNode::Update (BaseNode* source_node, float value) {

    // find index of node that changed
    for (auto & i : BaseNode::inputs_v) {
        if (i->target == source_node) {
            if (i->node_val != value) { // if value has changed from previous call

                // update sum and change
                sum_f += (value - i->node_val) * i->node_weight;
                i->node_val = value;

                // cascade changed result to all output nodes
                for (auto & targets : BaseNode::outputs_v)
                    targets->target->Update(this, sum_f);

            }


        }
    }
    return (-1);
}

std::string GraphNode::GetNodeType() {

    std::string result = "GraphNode";
    return (result);

}

float GraphNode::GetValue () {
    return (sum_f);
}

