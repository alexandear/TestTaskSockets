#include "stdafx.h"
#include "ClientXml.h"


ClientXml::ClientXml() : network(new ClientNetwork) { }


ClientXml::~ClientXml() {
    delete network;
}


void ClientXml::update(const std::string & command) {
    char m_buffer[ClientNetwork::DefaultBufferLength] = {};
    if (network->sendData(command.c_str()) > 0) {
        if (network->receiveData(m_buffer, ClientNetwork::DefaultBufferLength) > 0) {
            std::cout << "Client received: " << m_buffer << std::endl;
        }
        
    }
}
