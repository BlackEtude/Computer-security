//
// Created by pandemic on 05.12.17.
//

#include "bclient.h"
#define BUFFER_SIZE 4096

bclient::bclient(int port, char *path, char *message) {
    N = BN_new();
    e = BN_new();

    //load public key (N, e)
    read_publickey_from_file(path);

    //prepare new message
    BIGNUM *x = calculate_msg(message);
//    std::cout << "x: " << BN_bn2dec(x) << std::endl << std::endl;
    communicate_with_server(port, BN_bn2hex(x));
}

void bclient::read_publickey_from_file(char *path) {
    std::cout << "Reading key from: " << path << std::endl;
    std::string item_name;
    std::ifstream nameFileout;
    nameFileout.open(path);
    std::string line;

    auto *temp = new std::string[2];

    int i = 0;
    while(std::getline(nameFileout, line)) {
        temp[i] = line;
        i++;
    }

    const char *c = temp[0].c_str();
    BN_hex2bn(&N, c);
    c = temp[1].c_str();
    BN_hex2bn(&e, c);

    std::cout << "N: " << BN_bn2dec(N) << std::endl << std::endl;
    std::cout << "e: " << BN_bn2dec(e) << std::endl << std::endl;
}

BIGNUM* bclient::calculate_msg(char *msg) {
    std::string hashed_msg = sha256(msg);
//    std::cout << "sha256('" << msg << "'):" << hashed_msg << std::endl;

    const char *hmsg = hashed_msg.c_str();
    BIGNUM *m = BN_new();
    BN_hex2bn(&m, hmsg);
//    std::cout << "hmsg: " << BN_bn2dec(m) << std::endl << std::endl;

    hashed = BN_bn2dec(m);

    r = BN_new();
    BIGNUM *one = BN_new();
    BIGNUM *gcd = BN_new();
    BN_CTX *ctx = BN_CTX_new();
    BN_one(one);

    do {
        BN_rand_range(r, N);
        BN_gcd(gcd,r, N, ctx);
    }
    while(BN_cmp(gcd, one) != 0);

//    std::cout << "number: " << BN_bn2dec(r) << std::endl << std::endl;
//    std::cout << "gcd: " << BN_bn2dec(gcd) << std::endl << std::endl;

    BIGNUM *x = BN_new();
    BN_mod_exp(x, r, e, N, ctx);
    BN_mod_mul(x, m, x, N, ctx);
    return x;
}

void bclient::communicate_with_server(int port, char *msg) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char signed_msg[BUFFER_SIZE] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return;
    }

    //send msg to server
    send(sock, msg, strlen(msg), 0);
    //    std::cout << "Msg sent: " << msg << std::endl;

    //get signed from server
    valread = read(sock, signed_msg, BUFFER_SIZE);
//    std::cout << "Received: " << signed_msg << std::endl;


    unsign_msg(signed_msg);
}

std::string bclient::sha256(const std::string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

void bclient::unsign_msg(char *msg) {
    BIGNUM *from = BN_new();
    BN_hex2bn(&from, msg);

    BIGNUM *inverse = BN_new();
    BIGNUM *s = BN_new();

//    s = r.modInverse(big_int).multiply(muprime).mod(big_int)
    BN_CTX *ctx;
    ctx = BN_CTX_new();
    BN_mod_inverse(inverse, r, N, ctx);
    BN_mod_mul(s, inverse, from, N, ctx);

    if(bverfy(s))
        std::cout << "VERIFY: true" << std::endl;
    else
        std::cout << "VERIFY: false" << std::endl;
}

bool bclient::bverfy(BIGNUM *msg) {
    //verify:
//    s^e (mod N)
    BIGNUM *h = BN_new();
    BN_CTX *ctx;
    ctx = BN_CTX_new();

    BN_mod_exp(h, msg, e, N, ctx);
    std::cout << "computed: " << BN_bn2dec(h) << std::endl;
    std::cout << "original msg:" << hashed << std::endl;

    return strcmp(hashed, BN_bn2dec(h)) == 0;
}

int main(int argc, char*argv[]) {
    if(argc < 4) {
        std::cout << "Missing arguments" << std::endl;
        return -1;
    }

    //port, path to key, message
    auto *client = new bclient(atoi(argv[1]), argv[2], argv[3]);
    return 0;
}