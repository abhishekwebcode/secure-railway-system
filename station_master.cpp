// g++ station_master.cpp -o station_master.o -lcrypto -lssl -L/opt/homebrew/opt/openssl@3.1/lib -I/opt/homebrew/opt/openssl@3.1/include -std=c++11
// Run station_master 
// ./station_master.o

#include <iostream> 
#include <stdio.h> 
#include "server.h"
#include "client.h"
#include "des.h"

#define RPORT 8080 
#define SPORT 8081
using namespace std;

int main(int argc, char const * argv[]) {
    Server sm;
    sm.createSocket(RPORT);
    sm.bindAndListenSocket();
    char * rec = (char*) sm.acceptAndReceive();
    string seq = sm.receiveRSAMessage(rec);
    if(!seq.empty()) sm.sendAck();

    // unsigned char * cipher = server.acceptAndReceive();
    // DESDecrypt des;
    // unsigned char* p = des.decrypt(cipher);
    cout<<"Seq from train =: "<<seq<<endl;

    Client sm2;
    sm2.createSocket();
    sm2.addServerAddress("127.0.0.1", SPORT);
    sm2.doConnect();
    sm2.sendDESMessage("abhishek mathur says ok");
    string ack = sm2.getAck();

    cout<<"ACK received from platform : "<<ack<<endl;

    return 0;
}
