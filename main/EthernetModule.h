#ifndef ETHERNET_MODULE_H
#define ETHERNET_MODULE_H

#include <Ethernet.h>

class EthernetModule {
public:
    EthernetModule(byte* mac);
    bool initialize();
    float measureInternetSpeed(const char* host, const char* path);

private:
    byte* mac;
};

#endif
