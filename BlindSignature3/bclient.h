//
// Created by pandemic on 05.12.17.
//

#ifndef SIGNATURER_BCLIENT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 4096
#define SIGNATURER_BCLIENT_H


class bclient {
private:
    BIGNUM *N, *e, *r;
    BN_CTX *ctx;
    const char* hashed;

    void load_publickey_from_file(char *path);
    BIGNUM *calculate_msg();
    std::string sha256(std::string str);
    void communicate_with_server(int port);
    double exchange_messages(int port, char *msg);
    double message_exp(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus);
    double cpu_seconds();
    uint64_t rdtsc();
    uint64_t benchmark(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus);
    BIGNUM* modular_exponentation(BIGNUM *x, BIGNUM *y, BIGNUM *m);
    double array_average(std::vector<double> array);
    ~bclient();
public:
    bclient(int port, char* public_key_path);
};

#endif //SIGNATURER_BCLIENT_H
