//
// Created by pandemic on 05.12.17.
//

#ifndef SIGNATURER_BSERVER_H
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <string>
#include <random>
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
#include <cstring>
#include <memory>
#include <spqr.hpp>
#include <ctime>
#include <cstdio>
#include <chrono>
#define SIGNATURER_BSERVER_H

class bserver {
private:
    RSA *r = NULL;
    BIGNUM *num, *N, *d;
    BN_CTX *ctx;            //for BIGNUM temp variables used by library functions
    int ret;

    void generate_password(int length);
    std::string sha256(std::string str);
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
