#ifndef SERVERXML_H
#define SERVERXML_H

#include "stdafx.h"
#include "ServerNetwork.h"


class ServerXml  {

public:
    ServerXml(const string & fileXmlPath = "ServerXML.xml");
    ~ServerXml();    
    void update();
    string fileXmlPath() const { return m_fileXmlPath; }
    string & xmlFilePath() { return m_fileXmlPath; }
private:
    ServerNetwork m_network;

    enum Command {Add, Get, Set, Unknown};  
    bool parseCommand(const string & command, Command & type, 
                      string & tagName, string & tagValue);
    friend std::ostream & operator<<(std::ostream & os, Command type);

    string m_fileXmlPath;
    TiXmlDocument m_doc;
    string createXml(const string & command);
    
};

#endif // SERVERXML_H
