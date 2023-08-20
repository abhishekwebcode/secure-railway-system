// g++ platform.cpp -o platform.o -lcrypto -lssl -L/opt/homebrew/opt/openssl@1.1/lib -I/opt/homebrew/opt/openssl@1.1/include -std=c++11
// Run platform
// ./platform.o

#include <iostream> 
#include <stdio.h> 
#include "server.h"
#include "client.h"
#include "des.h"
#define RPORT 8081
using namespace std;

int main(int argc, char const * argv[]) {
    Server platform;
    platform.createSocket(RPORT);
    platform.bindAndListenSocket();
    unsigned char * cipher = platform.acceptAndReceive();
    DESDecrypt des;
    unsigned char* pt = des.decrypt(cipher);
    printf("Sequence from Station master =: %s\n",pt);
    platform.sendAck();
    return 0;
}
