#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include "stdafx.h"


class ClientNetwork {

public:
    static const std::string DefaultPort;
    static const std::string ServerAdress;
    static const int DefaultBufferLength = 512;

    ClientNetwork();
    ~ClientNetwork();
    int receiveData(char * receiveBuffer, int receiveBufferLength);
    int sendData(const char * sendBuffer);

private:
    SOCKET ConnectSocket;
    int iResult;

};

#endif // CLIENTNETWORK_H
