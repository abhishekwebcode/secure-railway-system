#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>
#include <openssl/rand.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "openssl_rsa.h"

#define PORT 8080
#define KEY_LENGTH  2048
#define PUB_EXP     3
#define PRINT_KEYS
#define WRITE_TO_FILE


#define BUFSIZE 64
using namespace std;

class Client {

    int sock, valread;
    struct sockaddr_in serv_addr;

    unsigned char out[BUFSIZE], back[BUFSIZE], ack[BUFSIZE];

    unsigned char *e;
    DES_cblock key = "1234567";
    DES_cblock seed = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    DES_key_schedule keysched;

public:

    Client() {
        this->sock = 0;
        this->e = out;
    }

    void log(string message) {
        cout << message << endl;
    }

    int createSocket() {
        log("Creating socket...");
        if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "\nCreation of socket failed\n";
            return -1;
        }
        log("Socket created, ready to go..");
        return 0;
    }

    int addServerAddress(string address, int port) {
        log("Adding address and port to the socket");

        this->serv_addr.sin_family = AF_INET;
        this->serv_addr.sin_port = htons(port);
        // Convert IPv4 and IPv6 addresses from text to binary form 
        if (inet_pton(AF_INET, address.c_str(), &(this->serv_addr).sin_addr) <= 0) {
            log("Address is invalid, unsupported format");
            return -1;
        }
        log("Address and port added successfully");
        return 0;
    }

    int doConnect() {
        log("Connecting to socket...");
        if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            log("Connection Failed");
            return -1;
        }
        log("Connected to socket successfully");
        return 0;
    }

    string getAck() {
        read(sock, ack, 1024);
        string res((char *) ack);
        return res;
    }

    void sendDESMessage(string message) {

        log("Sending message in encrypted form");


        memset(out, 0, sizeof(out));
        memset(back, 0, sizeof(back));

        RAND_seed(seed, sizeof(DES_cblock));

        DES_set_key((DES_cblock *) key, &keysched);

        log("Encrypting, plaintext : " + message);

        DES_ecb_encrypt((DES_cblock *) message.c_str(), (DES_cblock *) out, &keysched, DES_ENCRYPT);

        log("Encrypted form : ");
        while (*e) printf(" [%02x]", *e++);
        printf("\n");

        send(sock, out, sizeof(out), 0);

    }

    //*********RSA**********
    void sendRSAMessage(string message) {
        char public_key_pem[11] = "public_key";

        RSA *public_key = getRSAKey(public_key_pem, PUBLIC_KEY_PEM);

        char *encrypt = NULL;

        encrypt = (char *) malloc(RSA_size(public_key));

        int encrypt_length = public_encrypt(message.size(), (unsigned char *) message.c_str(),
                                            (unsigned char *) encrypt, public_key, RSA_PKCS1_OAEP_PADDING);
        if (encrypt_length == -1) {
            LOG("An error occurred in public_encrypt() method");
        }

        cout << "encrypted length : " << encrypt_length << endl;
        LOG("Data has been encrypted.");
        printf("Sending : %s\n", encrypt);

        send(sock, encrypt, strlen(encrypt), 0);

        printf("Encrypted message sent\n");

        free(public_key);
        free(encrypt);
        LOG("OpenSSL_RSA has been finished.");

    }


};