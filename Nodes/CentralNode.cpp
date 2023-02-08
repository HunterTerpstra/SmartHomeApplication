//
// Created by Ryan Chan on 2022-11-07.
//

#include "CentralNode.h"

void CentralNode::SendUpdate (std::string* device_ip, std::string* device_name, float device_value) {

    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;

    // create output string
    std::string msg = std::string();
    msg += "HTTP PATCH "; msg += "/";
    msg += *device_name;
    msg += "?value="; msg += std::to_string(device_value);

    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Output socket could not be created." << std::endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, device_ip->c_str(), &serv_addr.sin_addr)
        <= 0) {
        std::cerr << "Output address is invalid" << std::endl;
        return;
    }

    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
        return;

    send(sock, msg.c_str(), msg.size(), 0);

    // closing the connected socket
    close(client_fd);
}

CentralNode::CentralNode() : BaseNode(-1) {

}

int CentralNode::Update (BaseNode* source_node, float value) {
    SendUpdate(new std::string("192.168.0.168"), new std::string(std::to_string(source_node->GetID())), value);
    return (1);
}

std::string CentralNode::GetNodeType() {

    std::string result = "CentralNode";
    return (result);

}