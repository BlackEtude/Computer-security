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
#include <random>

#define BUFFER_SIZE 4096
#define SIGNATURER_SIGN_TEST_H


class sign_test {
public:
    BIGNUM *N, *d, *e, *r;
    BN_CTX *ctx;
    
    sign_test(char *priv_path, char *pub_path);
    void read_key_from_file(char *path, BIGNUM *x, BIGNUM *y);
    double sign_msg(BIGNUM *msg_to_sign, BIGNUM *expo, BIGNUM *modulus);

    void test();
    BIGNUM *generate_msg();
    std::string sha256(std::string str);
    double cpu_seconds();
    uint64_t rdtsc();
    uint64_t benchmark(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus);
    void modular_exponentation(BIGNUM *x, BIGNUM *y, BIGNUM *m);
    double array_var(std::vector<double> array);
    bool is_proper_diff(double var0, double var1);
};


#endif //SIGNATURER_SIGN_TEST_H
