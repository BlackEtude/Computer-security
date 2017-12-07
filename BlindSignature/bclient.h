//
// Created by pandemic on 05.12.17.
//

#ifndef SIGNATURER_BCLIENT_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <chrono>
#define SIGNATURER_BCLIENT_H


class bclient {
private:
    BIGNUM *N, *e, *r;
    BN_CTX *ctx;
    const char* hashed;

    void load_publickey_from_file(char *path);
    BIGNUM *calculate_msg(char *msg);
    std::string sha256(std::string str);
    void communicate_with_server(int port, char *msg);
    void remove_signature(char *msg_to_unsign);
    bool bverfy(BIGNUM *msg);
    ~bclient();
public:
    bclient(int port, char* public_key_path, char* message_to_sign);
};

#endif //SIGNATURER_BCLIENT_H
