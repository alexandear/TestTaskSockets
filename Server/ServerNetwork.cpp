#include "stdafx.h" 
#include "ServerNetwork.h"


const char * ServerNetwork::DefaultPort = "27015";


ServerNetwork::ServerNetwork() : ListenSocket(INVALID_SOCKET), ClientSocket(INVALID_SOCKET) {
    using std::cout;
    using std::endl;

    WSADATA wsaData;
    struct addrinfo *result = nullptr;
    struct addrinfo hints;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed with error: " << iResult << endl;
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(nullptr, DefaultPort, &hints, &result);
    if ( iResult != 0 ) {
        cout << "getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        cout << "socket failed with error: " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        cout << "bind failed with error: " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

     // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        cout << "listen failed with error: " << WSAGetLastError() << endl;
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, nullptr, nullptr);
    if (ClientSocket == INVALID_SOCKET) {
        cout << "accept failed with error: " << WSAGetLastError() << endl;
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // No longer need server socket
    closesocket(ListenSocket);
}


ServerNetwork::~ServerNetwork() {
    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
}


int ServerNetwork::receiveData(char * receiveBuffer, int receiveBufferLength) {
    iResult = recv(ClientSocket, receiveBuffer, receiveBufferLength, 0);
    if (iResult == 0) {
        std::cout << "Connection closing..." << std::endl;
        exit(1);
    } else if (iResult < 0) {
        std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }
    return iResult;
}


int ServerNetwork::sendData(const char * sendBuffer) {
    iResult = send( ClientSocket, sendBuffer, static_cast<int>(std::strlen(sendBuffer)), 0 );
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }
    return iResult;
}
