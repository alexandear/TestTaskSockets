#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include "stdafx.h"


class ServerNetwork {

public:
    static const char * DefaultPort;
    static const int DefaultBufferLength = 512;

    ServerNetwork();
    ~ServerNetwork();
    int receiveData(char * receiveBufffer, int receiveBuffferLength);
    int sendData(const char * sendBuffer);
    
private:
    SOCKET ClientSocket;
    SOCKET ListenSocket;
    int iResult;

};

#endif // SERVERNETWORK_H
