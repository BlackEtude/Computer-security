//
// Created by pandemic on 05.12.17.
//

#include "bserver.h"
#define BUFFER_SIZE 4096

bserver::bserver() {
    ret = 0;
}

void bserver::setup(char* path) {
    // 1. generate rsa key
    big_int = BN_new();
    ret = BN_set_word(big_int, RSA_F4);
    if(ret != 1) {
        std::cout << "Error. Ending" << std::endl;
    }

    generate_key_pair(2048, path);
    generate_key_pair(4096, path);
    generate_key_pair(8192, path);
    generate_key_pair(16384, path);
    generate_password(16);
}

void bserver::generate_key_pair(int key_length, char* path) {
    r = RSA_new();
    ret = RSA_generate_key_ex(r, key_length, big_int, NULL);
    if(ret != 1) {
        std::cout << "Error. Ending" << std::endl;
    }

    const BIGNUM *N = BN_new();
    const BIGNUM *d = BN_new();
    const BIGNUM *e = BN_new();
    RSA_get0_key(r, &N, &e, &d);

//    std::cout << "N: " << BN_bn2dec(N) << std::endl << std::endl;
//    std::cout << "d: " << BN_bn2dec(d) << std::endl << std::endl;
//    std::cout << "e: " << BN_bn2dec(e) << std::endl << std::endl;

    char p[30];
    FILE *file;
    std::string s = std::to_string(key_length);
    char const *length = s.c_str();

    // 2. save public key
    memset(p, 0, sizeof p);
    strcat(p, path);
    strcat(p, "public");
    strcat(p, length);

    file = fopen(p , "w+");
    BN_print_fp(file, N);
    fprintf(file, "\n");
    BN_print_fp(file, e);
    fclose(file);

    // 3. save private key
    memset(p, 0, sizeof p);
    strcat(p, path);
    strcat(p, "private");
    strcat(p, length);

    file = fopen(p , "w+");
    BN_print_fp(file, N);
    fprintf(file, "\n");
    BN_print_fp(file, d);
    fclose(file);

    std::cout << "Length " << key_length << " generated" << std::endl;

}

void bserver::generate_password(int length) {
    const std::string::value_type allowed_chars[] {"123456789ABCDFGHJKLMNPQRSTVWXZabcdfghjklmnpqrstvwxz!@#$%^&*()"};
    thread_local std::default_random_engine randomEngine(std::random_device{}());
    thread_local std::uniform_int_distribution<int> randomDistribution(0, sizeof(allowed_chars) - 1);
    std::string str_pass(length ? length : 16, '\0');

    for (std::string::value_type& c : str_pass)
        c = allowed_chars[randomDistribution(randomEngine)];

    std::string hashed_pass = sha256(str_pass);
    std::cout << "Pass: " << str_pass << std::endl;

    // save password
    std::string input;
    std::ofstream out("config_server");
    out << hashed_pass;
}

void bserver::server_listen(char *password, int port, char* key_path) {
    //if pass is not correct -> end
    if(!check_password(password)) {
        std::cout << "Given password is not correct. Aborting..." << std::endl;
        return;
    }

    //load private key (N, d)
    read_key_from_file(key_path);

    //server_listen
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("server_listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //get message from client
    valread = read(new_socket, buffer, BUFFER_SIZE);
    BIGNUM *m = BN_new();
    BN_hex2bn(&m, buffer);
//    std::cout << "received x: " << BN_bn2hex(m) << std::endl << std::endl;

    //check if in group
    if(!check_if_in_group(m)) {
        return;
    }

    //send signed msg to client
    char* signed_msg = sign(m);
    send(new_socket, signed_msg, strlen(signed_msg), 0);
//    std::cout << "Msg sent to client: " << signed_msg << std::endl;
}

bool bserver::check_if_in_group(BIGNUM *num) {
    BIGNUM *gcd = BN_new();
    BIGNUM *one = BN_new();
    BN_CTX *ctx;
    ctx = BN_CTX_new();
    BN_gcd(gcd, num, N, ctx);
    return BN_cmp(one, gcd) != 0;
}

bool bserver::check_password(char *user_pass) {
    //load pass from file
    std::string line;
    std::ifstream myfile ("config_server");
    if (myfile.is_open()) {
        while (std::getline (myfile, line));
        myfile.close();
    }
    std::string user_hashed = sha256(user_pass);

    return line.compare(user_hashed) == 0;
}

char* bserver::sign(BIGNUM* msg_to_sign) {
    //load N, d, e

    BIGNUM *result = BN_new();
    BN_CTX *ctx;
    ctx = BN_CTX_new();

    //got this from client
    BN_mod_exp(result, msg_to_sign, d, N, ctx);
//    std::cout << "result: " << BN_bn2dec(result) << std::endl << std::endl;
    return BN_bn2hex(result);

}

void bserver::read_key_from_file(char *path) {
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
    BN_hex2bn(&d, c);

//    std::cout << "N: " << BN_bn2dec(N) << std::endl << std::endl;
//    std::cout << "d: " << BN_bn2dec(d) << std::endl << std::endl;
}

std::string bserver::sha256(std::string str) {
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

bserver::~bserver() {
    RSA_free(r);
    BN_free(big_int);
}

int main(int argc, char*argv[]) {
    if(argc < 3) {
        std::cout << "Missing arguments" << std::endl;
        return -1;
    }

    auto *server = new bserver();

    if(strcmp(argv[1], "setup") == 0) {
        std::cout << "setup mode started" << std::endl;
        server->setup(argv[2]);
    }

    else if(strcmp(argv[1], "sign") == 0) {
        std::cout << "sign mode started\n" << std::endl;
        server->server_listen(argv[2], atoi(argv[3]), argv[4]);
    }
    else {
        std::cout << "Wrong parameter. Type 'setup' or 'sign'" << std::endl;
    }

    return 0;
}

