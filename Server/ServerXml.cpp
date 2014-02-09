#include "stdafx.h" 
#include "ServerXml.h"


ServerXml::ServerXml() : m_network(new ServerNetwork), m_doc(TiXmlDocument("file.xml")) {

    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    m_doc.LinkEndChild(decl);
}


ServerXml::~ServerXml() {
    delete m_network;
}


void ServerXml::update() {
    char buffer[ServerNetwork::DefaultBufferLength] = {};
    if (m_network->receiveData(buffer, ServerNetwork::DefaultBufferLength) > 0) {
        string reply = createXml(string(buffer));
        m_network->sendData(reply.c_str());
    }
}


string ServerXml::createXml(const string & command) {
    using std::cout;
    using std::endl;

    string reply;
    string tagNameStr, tagValueStr;
    Command type;
    reply = "ok";
    if (parseCommand(command, type, tagNameStr, tagValueStr)) {

        cout << "Command: \"" << type << "\" tag name: \"" << tagNameStr << "\"";
        cout << (type == Get ? "\n" : " value: \"" + tagValueStr + "\"\n");

        TiXmlElement * tagName;
        TiXmlText * tagValue;
        switch (type) {
        case Add:
            tagName = new TiXmlElement(tagNameStr);
            m_doc.LinkEndChild(tagName);
            tagValue = new TiXmlText(tagValueStr);
            tagName->LinkEndChild(tagValue);
            break;

        case Get:
        case Set:
            tagName = m_doc.FirstChildElement(tagNameStr);
            if (tagName) {
                tagValue = tagName->FirstChild()->ToText();
                if (tagValue) {
                    if (type == Get) {
                        reply = tagValue->ValueStr();
                    } else {
                        tagValue->SetValue(tagValueStr);
                    }
                }
            }
            break;
        }
        if (!m_doc.SaveFile()) {
            reply = "Can't create xml file";
        }
    } else {
        reply = "No such command";
    }
    return reply;
}


bool ServerXml::parseCommand(const string & command, Command & type,
                             string & tagName, string & tagValue) {

    std::smatch what;
    std::regex regAddSet(" *(add|set) *<(.*)> *<(.*)> *");
    std::regex regGet(" *(get) *<(.*)> *");

    bool successAddSet = std::regex_search(command, what, regAddSet);
    bool successGet = std::regex_search(command, what, regGet);

    if (successAddSet || successGet) {
        string commandStr = what[1];
        if (commandStr == "add") {
            type = Command::Add;
            tagValue = what[3];
        } else if (commandStr == "set") {
            type = Command::Set;
            tagValue = what[3];
        } else if (commandStr == "get") {
            type = Command::Get;
        }
        tagName = what[2];
        
        return true;
    }
    return false;
}


std::ostream & operator<<(std::ostream & os, ServerXml::Command type) {
    if (type == ServerXml::Command::Add) {
        os << "add";
    } else if (type == ServerXml::Command::Get) {
        os << "get";
    } else if (type == ServerXml::Command::Set) {
        os << "set";
    } else {
        os << "unknown";
    }
    return os;
}
