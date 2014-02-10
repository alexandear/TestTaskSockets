#include "stdafx.h"
#include "ClientXml.h"


ClientXml::ClientXml() { }


ClientXml::~ClientXml() { }


void ClientXml::update(const std::string & command) {
    char m_buffer[ClientNetwork::DefaultBufferLength] = {};
    if (m_network.sendData(command.c_str()) > 0) {
        if (m_network.receiveData(m_buffer, ClientNetwork::DefaultBufferLength) > 0) {
            std::cout << "Client received: " << m_buffer << std::endl;
        }
        
    }
}
