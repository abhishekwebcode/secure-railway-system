#ifndef RSA_ALGORITHM_H
#define RSA_ALGORITHM_H

#define KEY_LENGTH       2048
#define PUBLIC_EXPONENT  59     //Public exponent should be a prime number.
#define PUBLIC_KEY_PEM   1
#define PRIVATE_KEY_PEM  0

#define LOG(x)               \
        cout << x << endl;   \

RSA * getRSAKey(char *file_name, int pem_type);
/*
 * @brief   create_RSA function creates public key and private key file
 *
 */
 RSA * create_RSA(RSA *keypair, int pem_type, char *file_name);

/*
 * @brief   public_ecrypt function encrypts data.
 * @return  If It is fail, return -1
 */
 int public_encrypt(int flen, unsigned char* from, unsigned char *to, RSA* key, int padding);

/*
 * @brief   private_decrypt function decrypt data.
 * @return  If It is fail, return -1
 */
 int private_decrypt(int flen, unsigned char* from, unsigned char *to, RSA* key, int padding);

/*
 * @brief   create_ecrypted_file function creates .bin file. It contains encrypted data.
 */
 void create_encrypted_file(char* encrypted, RSA * key_pair);

#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include "openssl_rsa.h"

using namespace std;



RSA * getRSAKey(char *file_name, int pem_type){
        RSA   *rsa = NULL;
        FILE  *fp  = NULL;

         fp = fopen(file_name, "rb");
         if(pem_type == PUBLIC_KEY_PEM) {
             PEM_read_RSAPublicKey(fp, &rsa, NULL, NULL);
         }
         else if(pem_type == PRIVATE_KEY_PEM) {
             PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
         }
        fclose(fp);

        return rsa;
}

RSA * create_RSA(RSA * keypair, int pem_type, char *file_name) {

    RSA   *rsa = NULL;
    FILE  *fp  = NULL;

    if(pem_type == PUBLIC_KEY_PEM) {

        fp = fopen(file_name, "w");
        PEM_write_RSAPublicKey(fp, keypair);
        fclose(fp);

        fp = fopen(file_name, "rb");
        PEM_read_RSAPublicKey(fp, &rsa, NULL, NULL);
        fclose(fp);

    }
    else if(pem_type == PRIVATE_KEY_PEM) {

        fp = fopen(file_name, "w");
        PEM_write_RSAPrivateKey(fp, keypair, NULL, NULL, 0, NULL, NULL);
        fclose(fp);

        fp = fopen(file_name, "rb");
        PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
        fclose(fp);

    }

    return rsa;
}

int public_encrypt(int flen, unsigned char* from, unsigned char* to, RSA* key, int padding) {
    
    int result = RSA_public_encrypt(flen, from, to, key, padding);
    return result;
}

int private_decrypt(int flen, unsigned char* from, unsigned char* to, RSA* key, int padding) {

    int result = RSA_private_decrypt(flen, from, to, key, padding);
    return result;
}

void create_encrypted_file(char* encrypted, RSA* key_pair) {

    FILE* encrypted_file = fopen("encrypted_file.bin", "w");
    fwrite(encrypted, sizeof(*encrypted), RSA_size(key_pair), encrypted_file);
    fclose(encrypted_file);
}


#endif //RSA_ALGORITHM_H
