//
// Created by Ryan Chan on 2022-11-25.
//

#include "NodePipe.h"

// instantiate static variables
char NodePipe::is_created = 0;
NodePipe* NodePipe::self = nullptr;

NodePipe* NodePipe::instance(int self_index) {

    // singleton creator
    if (is_created == 0) {

        self = new NodePipe();
        is_created = 1;
    }

    self->add_process(self_index);
    return (self);

}

NodePipe::NodePipe() {
    queues = std::vector<struct ProcessPipe*>();
}

NodePipe::~NodePipe() = default;

void NodePipe::add_process(int self_index) {

    // check if vector size is large enough to accomodate requested index, if not, expand
    if (queues.size() < self_index + 1)
        queues.resize(self_index + 1);

    if (queues.at(self_index) == nullptr) { // index has not been registered, register as new point

        auto* package = new struct ProcessPipe();
        package->in_pipe = new std::queue<struct PackageNode*>();

        queues.insert(queues.begin() + self_index, package);
    } // index has already been reserved, do nothing
}

void NodePipe::write (int target_index, struct PackageNode* content) {

    // check bounds
    if (queues.size() - 1 < target_index)
        queues.resize(target_index + 1);

    // check if queue has been registered, if yes, ...
    if (queues.at(target_index) != nullptr) {

        // if more than 1 thread is expected to write to a pipe at a time, use mutexes but for now is unnecessary
        queues.at(target_index)->in_pipe->push(content);

    }

}

struct NodePipe::PackageNode* NodePipe::read (int target_index) {

    // check bounds
    if (queues.size() - 1 < target_index)
        queues.resize(target_index + 1);

    // read from pipe
    struct ProcessPipe* target_pipe = queues.at(target_index);
    if (target_pipe != nullptr && !target_pipe->in_pipe->empty()) {

        struct PackageNode* package = target_pipe->in_pipe->front();
        target_pipe->in_pipe->pop();
        return (package);

    }

    return (nullptr);
}

struct NodePipe::PackageNode* NodePipe::peek (int target_index) {

    // check bounds
    if (queues.size() - 1 < target_index)
        queues.resize(target_index + 1);

    // peek
    struct ProcessPipe* target_pipe = queues.at(target_index);
    // queues at 1 is always nullptr
    if (target_pipe != nullptr && !target_pipe->in_pipe->empty()) {

        struct PackageNode* package = target_pipe->in_pipe->front();
        return (package);

    }

    return (nullptr);
}
