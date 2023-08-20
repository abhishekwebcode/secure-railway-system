#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <openssl/des.h>
#include <openssl/rand.h>
#include <arpa/inet.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "openssl_rsa.h"

#define BUFSIZE 64
using namespace std;

class Server {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    unsigned char buffer[BUFSIZE] = {0};
    unsigned char *e;
public:
    Server() {
        this->e = buffer;
    }

    void log(string message) {
        cout << message << endl;
    }

    void createSocket(int port) {
        log("Creating socket");
        // Creating socket file descriptor 
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        log("Socket created successfully");
        // Forcefully attaching socket to the port 8080 
        log("Adding options for socket");
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        log("Options added successfully");
    }

    void bindAndListenSocket() {
        // Forcefully attaching socket to the port 8080 
        log("Binding socket to port");
        if (bind(server_fd, (const struct sockaddr *) &address, (socklen_t) sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        log("Socket has been binded");
        log("Listening for client");
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
    }

    unsigned char *acceptAndReceive() {
        memset(buffer, 0, sizeof(buffer));


        log("Waiting for client request, Please wait...");
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                 (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read(new_socket, (DES_cblock *) buffer, sizeof(buffer));

        log("Request received...");
        return buffer;
    }

    void sendAck() {
        string ack = "ok sent by abhishek mathur";
        send(new_socket, ack.c_str(), ack.size(), 0);
    }

    void removeFile(char *filename) {
        if (remove(filename) == 0) {
            puts("File successfully deleted");
        }
    }

    string receiveRSAMessage(char *encrypt) {

        LOG("OpenSSL_RSA has been started.");

        RSA *private_key;
        RSA *public_key;

        char *decrypt = NULL;

        RSA *keypair = NULL;
        BIGNUM *bne = NULL;
        int ret = 0;

        char private_key_pem[12] = "private_key";
        char public_key_pem[11] = "public_key";

        LOG(KEY_LENGTH);
        LOG(PUBLIC_EXPONENT);

        // RSA *keypair = RSA_generate_key(KEY_LENGTH, PUBLIC_EXPONENT, NULL, NULL); //Old

        bne = BN_new();
        ret = BN_set_word(bne, PUBLIC_EXPONENT);
        if (ret != 1) {
            // goto free_stuff;
            LOG("An error occurred in BN_set_word() method");
        }
        keypair = RSA_new();
        ret = RSA_generate_key_ex(keypair, KEY_LENGTH, bne, NULL);
        if (ret != 1) {
            // goto free_stuff;
            LOG("An error occurred in RSA_generate_key_ex() method");
        }
        LOG("Generate key has been created.");

        private_key = create_RSA(keypair, PRIVATE_KEY_PEM, private_key_pem);
        LOG("Private key pem file has been created.");

        public_key = create_RSA(keypair, PUBLIC_KEY_PEM, public_key_pem);
        LOG("Public key pem file has been created.");

        string buff(encrypt);
        cout << "Received is : " << buff << endl;
        int encrypt_length = buff.size();

        cout << "encrypted length : " << encrypt_length << endl;

        decrypt = (char *) malloc(encrypt_length);

        int decrypt_length = private_decrypt(sizeof(encrypt), (unsigned char *) encrypt,
                                             (unsigned char *) decrypt, private_key, RSA_PKCS1_OAEP_PADDING);

        if (decrypt_length == -1) {
            LOG("An error occurred in private_decrypt() method");
        }
        LOG("Data has been decrypted.");

        printf("Decrypted message: %s\n", decrypt);
        string res((char *) decrypt);
        return res;
    }

};
