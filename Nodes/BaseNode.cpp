//
// Created by Ryan Chan on 2022-11-07.
//

#include "BaseNode.h"

BaseNode::BaseNode(int node_id) : self_id(node_id) {
    inputs_v = std::vector<struct NodeRecord*>();
    outputs_v = std::vector<struct NodeRecord*>();
}

BaseNode::~BaseNode() = default;

int BaseNode::GetID () {
    return (self_id);
}

// add backward pointing reference
void BaseNode::AddInputNode (BaseNode* target, float target_weight) {

    // prevent nodes from being added twice
    for (auto & inputs_s : inputs_v) {
        if (inputs_s->target == target) return;
    }

    // create record for own
    struct NodeRecord* curr_record = new struct NodeRecord;
    curr_record->target = target;
    curr_record->node_weight = target_weight;
    curr_record->node_val = 0.0f;
    inputs_v.push_back(curr_record);

    // create mirrored reference for target node
    struct NodeRecord* alien_record = new struct NodeRecord;
    alien_record->target = this;
    alien_record->node_weight = 1.0; // max weighting, weighting does not affect outgoing values
    alien_record->node_val = 0.0f;
    target->outputs_v.push_back(alien_record);

    printf("Node %d added to node %d as input.\n", target->self_id, this->self_id);
}

// add forward pointing reference
void BaseNode::AddOutputNode (BaseNode* target, float self_weight) {

    // prevent nodes from being added twice
    for (auto & outputs_s : outputs_v) {
        if (outputs_s->target == target) return;
    }

    struct NodeRecord* curr_record = new struct NodeRecord;
    curr_record->target = target;
    curr_record->node_weight = 1.0f;
    curr_record->node_val = 0.0f;
    outputs_v.push_back(curr_record);

    // create mirrored reference for target node
    struct NodeRecord* alien_record = new struct NodeRecord;
    alien_record->target = this;
    alien_record->node_weight = self_weight; // weight record for self uses self_weight
    alien_record->node_val = 0.0f;
    target->inputs_v.push_back(alien_record);

    printf("Node %d added to node %d as output.\n", target->self_id, this->self_id);
}

void BaseNode::RemoveInputNode (BaseNode* target) {

    // find if any of its nodes match and if it does, remove
    for (int i = 0; i < inputs_v.size(); i++) {
        if (inputs_v.at(i)->target == target) {

            target->RemoveOutputNode(this); // remove corresponding pointer in node
            inputs_v.erase(inputs_v.begin() + i);
            printf("Input node %d has been removed from node %d.\n", target->self_id, this->self_id);
            return;
        }
    }
}
void BaseNode::RemoveOutputNode (BaseNode* target) {
    for (int i = 0; i < outputs_v.size(); i++) {
        if (outputs_v.at(i)->target == target) {

            target->RemoveInputNode(this); // remove corresponding pointer in node
            outputs_v.erase(outputs_v.begin() + i);
            printf("Output node %d has been removed from node %d.\n", target->self_id, this->self_id);
            return;
        }
    }
}

std::vector<struct BaseNode::NodeRecord*>* BaseNode::GetInputNodes () {

    return (&inputs_v);

}

std::vector<struct BaseNode::NodeRecord*>* BaseNode::GetOutputNodes () {

    return (&outputs_v);

}

// value of input node has changed
int BaseNode::Update (BaseNode* source_node, float value) {

    printf("BaseNode %d has received a value of %.2f from node %d.\n", this->self_id, value, source_node->self_id);
    return (0);

}

void BaseNode::Firehose () {

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~ Firehose for node %2d ~~~~~~~~~~~~~~~~~~~~~~~\n", this->self_id);
    int counter = 0;

    printf("Inputs: \n");
    for (auto & node_record_instance : inputs_v)
        printf("%2d: Node %d\n", ++counter, node_record_instance->target->self_id);

    counter = 0;
    printf("Outputs: \n");
    for (auto & node_record_instance : outputs_v)
        printf("%2d: Node %d\n", ++counter, node_record_instance->target->self_id);

    printf("~~~~~~~~~~~~~~~~~~~~~~~ End of firehose %2d ~~~~~~~~~~~~~~~~~~~~~~~\n\n", this->self_id);

}

std::string BaseNode::GetNodeType() {

    std::string result = "BaseNode";
    return (result);

}
