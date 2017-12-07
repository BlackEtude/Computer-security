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
#define SIGNATURER_BSERVER_H

class bserver {
    RSA *r = NULL;
    BIGNUM *big_int, *N, *d;
    int ret;
public:
    bserver();
    void setup(char* path);
    void generate_key_pair(int key_length, char *path);
    void server_listen(char *password, int port, char *key_path);
    bool check_password(char* user_pass);
    char* sign(BIGNUM* msg);
    bool check_if_in_group(BIGNUM *num);
    void generate_password(int length);
    void read_key_from_file(char *path);
    std::string sha256(std::string str);
    ~bserver();
};


#endif //SIGNATURER_BSERVER_H
