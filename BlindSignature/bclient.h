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
#define SIGNATURER_BCLIENT_H


class bclient {
public:
    bclient(int port, char* public_key_path, char* message_to_sign);
    void read_publickey_from_file(char *path);
    BIGNUM *calculate_msg(char *msg);
    std::string sha256(std::string str);
    void communicate_with_server(int port, char *msg);
    void unsign_msg(char *msg);
    bool bverfy(BIGNUM *msg);
    ~bclient();
private:
    BIGNUM *N, *e;
    BIGNUM *r;
    const char* hashed;
};

#endif //SIGNATURER_BCLIENT_H
