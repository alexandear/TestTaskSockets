#include "stdafx.h"
#include "ClientXml.h"


int main() {

    using std::cout;
    using std::cin;
    using std::string;
    using std::getline;

    ClientXml client;
    string command;
    cout << "Enter command (empty string to exit): ";
    getline(cin, command);
    while(!command.empty()) {
        client.update(command);
        cout << "Enter command (empty string to exit): ";
        getline(cin, command);
    }

    return 0;
}
