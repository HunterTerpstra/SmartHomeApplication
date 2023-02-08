//
// Created by Ryan Chan on 2022-11-07.
//

#ifndef INET
#define INET
#include <arpa/inet.h>
#endif

#ifndef UNISTD
#define UNISTD
#include <unistd.h>
#endif

#ifndef SOCKET
#define SOCKET
#include <sys/socket.h>
#endif


#ifndef BASENODE_INCLUDE
#define BASENODE_INCLUDE

#include "BaseNode.h"

#endif /* BASENODE_INCLUDE */

#ifndef INC_3307_CENTRALNODE_H
#define INC_3307_CENTRALNODE_H

#define PORT 80

/**
 * Acts as a central anchor point for nodes to attach onto. All communication going out of the system takes place here.
 * Changes will eventually cascade into this node, which then broadcasts through the appropriate protocol.
 */
class CentralNode : public BaseNode {
private:

    /**
     * @brief Broadcasts the updated status of a node through HTTP to a correlated IP address through the format "HTTP PATCH [ip]?value=[value]"
     * @param device_ip IP address of target device
     * @param device_name Registered name of device
     * @param device_value Updated value to send
     * @return
     */
    void SendUpdate (std::string* device_ip, std::string* device_name, float device_value);

public:

    /**
     * @brief Constructs central node object with ID of -1
     */
    CentralNode();

    /**
     * @brief Changes have cascaded from input nodes to output nodes, changes are sent to respective nodes
     * @param source_node Source of change, the output device
     * @param value Updated value
     * @return Success state
     */
    int Update (BaseNode* source_node, float value) override;

    /**
     * @brief Returns the name of this node's type, overrides BaseNode
     * @return Node type name
     */
    std::string GetNodeType () override;

};


#endif //INC_3307_CENTRALNODE_H
