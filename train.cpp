// g++ train.cpp -o train.o -lcrypto -lssl -L/opt/homebrew/opt/openssl@1.1/lib -I/opt/homebrew/opt/openssl@1.1/include -std=c++11
// Run train
// ./train.o

#include <iostream>
#include <stdio.h> 
#include "client.h"
#define PORT 8080 
using namespace std;

int main(int argc, char const * argv[]) {
    Client train;
    train.createSocket();
    train.addServerAddress("127.0.0.1", PORT);
    train.doConnect();
    train.sendRSAMessage("A-B-C-D-E-F-Z");
    cout<<"Ack received from station master : "<<train.getAck();
    return 0;
}