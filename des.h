#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>
#include <openssl/rand.h>

#define BUFSIZE 64 
using namespace std;

class DESDecrypt {
    DES_cblock key = "1234567";
    DES_cblock seed = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    DES_key_schedule keysched;
    unsigned char back[BUFSIZE];
    public:

        void log(string message) {
            cout << "<Server>: " << message << endl;
        }

    unsigned char* decrypt(unsigned char* cipher) {

    memset(back, 0, sizeof(back));
    RAND_seed(seed, sizeof(DES_cblock));
 
    DES_set_key((DES_cblock *)key, &keysched);

    DES_ecb_encrypt((DES_cblock *)cipher,(DES_cblock *)back, &keysched, DES_DECRYPT);
 
    return back;

    }
};
