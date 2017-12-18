//
// Created by pandemic on 17.12.17.
//

#ifndef SIGNATURER_SIGN_TEST_H
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
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
#include <netinet/in.h>
#include <openssl/crypto.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <vector>
#include <chrono>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 4096
#define SIGNATURER_SIGN_TEST_H


class sign_test {
public:
    BIGNUM *N, *d, *e, *r;
    BN_CTX *ctx;
    sign_test();
    void read_key_from_file(char *path);
    void load_publickey_from_file(char *path);
    double sign_msg(BIGNUM *msg_to_sign);

    void test();
    BIGNUM *calculate_msg();
    std::string sha256(std::string str);
    double message_exp(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus);
    double cpu_seconds();
    uint64_t rdtsc();
    uint64_t local_benchmark(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus);
    BIGNUM* modular_exponentation(BIGNUM *x, BIGNUM *y, BIGNUM *m);
    double array_average(std::vector<double> array);
};


#endif //SIGNATURER_SIGN_TEST_H
