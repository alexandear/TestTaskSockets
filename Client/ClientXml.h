#ifndef CLIENTXML_H
#define CLIENTXML_H

#include "stdafx.h"
#include "ClientNetwork.h"


class ClientXml {

public:
    ClientXml();
    ~ClientXml();
    void update(const std::string & command);

private:
    ClientNetwork * network;

};

#endif // CLIENTXML_H