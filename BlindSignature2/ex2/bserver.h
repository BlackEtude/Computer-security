//
// Created by pandemic on 05.12.17.
//

#ifndef SIGNATURER_BSERVER_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <random>
#include <memory>
#include <spqr.hpp>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define BUFFER_SIZE 4096
#define LENGTH 32
#define ITERS 5000
#define HASH_LEN 16
#define BN_FLG_CONSTTIME   0x04
#define BN_FLG_SECURE      0x08
#define SIGNATURER_BSERVER_H

class bserver {
private:
    RSA *r = NULL;
    BIGNUM *num, *N, *d;
    BN_CTX *ctx;            //for BIGNUM temp variables used by library functions
    BN_MONT_CTX *ctx_mont;
    int ret;

    void generate_password();
    unsigned char* generate_random_bytes(int size);
    char* code_base64(unsigned char *buff, int size);
    void generate_key_pair(int key_length, char *path_to_save);
    bool is_server_password_valid(char *user_pass);
    char* sign_msg(BIGNUM *msg);
    bool is_msg_in_group(BIGNUM *num);
    void read_key_from_file(char *path);
public:
    bserver();
    void setup(char* path);
    void communicate_with_client(char *password, int port, char *key_path);
    ~bserver();
};


#endif //SIGNATURER_BSERVER_H
