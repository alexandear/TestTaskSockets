#ifndef CLIENTXML_H
#define CLIENTXML_H

#include "stdafx.h"
#include "ClientNetwork.h"


class ClientXml {

public:
    ClientXml();
    void update(const std::string & command);

private:
    ClientNetwork m_network;

};

#endif // CLIENTXML_H
