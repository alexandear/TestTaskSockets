#include "stdafx.h"
#include "ClientNetwork.h"


const char * ClientNetwork::DefaultPort = "27015";


ClientNetwork::ClientNetwork() : ConnectSocket(INVALID_SOCKET) {
    using std::cout;
    using std::endl;

    WSADATA wsaData;
    struct addrinfo *result = nullptr, *ptr = nullptr, hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed with error: " << iResult << endl;
        exit(1);
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", DefaultPort, &hints, &result);
    if ( iResult != 0 ) {
        cout << "getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != nullptr; ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            cout << "socket failed with error: " << WSAGetLastError() << endl;
            WSACleanup();
            exit(1);
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Unable to connect to server!" << endl;
        WSACleanup();
        exit(1);
    }

}


ClientNetwork::~ClientNetwork() {
    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }

    closesocket(ConnectSocket);
    WSACleanup();
}


int ClientNetwork::receiveData(char * receiveBuffer, int receiveBufferLength)  {
    iResult = recv(ConnectSocket, receiveBuffer, receiveBufferLength, 0);
    if ( iResult == 0 )
        std::cout << "Connection closed" << std::endl;
    else if ( iResult < 0 ) {
        std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
    }
    return iResult;
}


int ClientNetwork::sendData(const char * sendBuffer) {
    iResult = send( ConnectSocket, sendBuffer, static_cast<int>(std::strlen(sendBuffer)), 0 );
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }
    return iResult;
}
