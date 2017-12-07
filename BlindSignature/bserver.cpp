//
// Created by pandemic on 05.12.17.
//

#include "bserver.h"
#define BUFFER_SIZE 4096

bserver::bserver() {
    num = BN_new();
    ctx = BN_CTX_new();
}

void bserver::setup(char* path) {
    // Generate rsa key
    ret = BN_set_word(num, RSA_F4);         //num == e
    if(ret != 1) {
        std::cout << "Error. Ending" << std::endl;
    }

    generate_password(16);

    // Generate key pairs
    generate_key_pair(2048, path);
//    generate_key_pair(4096, path);
//    generate_key_pair(8192, path);
    generate_key_pair(16384, path);
}

void bserver::generate_key_pair(int key_length, char* path_to_save) {
    // Measure elapsed time
    auto start = std::chrono::high_resolution_clock::now();

    r = RSA_new();
    ret = RSA_generate_key_ex(r, key_length, num, NULL);
    if(ret != 1) {
        std::cout << "Error. Ending" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Generate " << key_length << " keys time: ";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << std::endl;

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

    // Save public key
    memset(p, 0, sizeof p);
    strcat(p, path_to_save);
    strcat(p, "public");
    strcat(p, length);

    file = fopen(p , "w+");
    BN_print_fp(file, N);
    fprintf(file, "\n");
    BN_print_fp(file, e);
    fclose(file);

    // Save private key
    memset(p, 0, sizeof p);
    strcat(p, path_to_save);
    strcat(p, "private");
    strcat(p, length);

    file = fopen(p , "w+");
    BN_print_fp(file, N);
    fprintf(file, "\n");
    BN_print_fp(file, d);
    fclose(file);

    std::cout << "Key " << key_length << " generated" << std::endl << std::endl;
}

void bserver::generate_password(int length) {
    //TODO problems with some of these chars: !@#$%^&*()
    const std::string::value_type allowed_chars[] {"123456789ABCDFGHJKLMNPQRSTVWXZabcdfghjklmnpqrstvwxz"};
    thread_local std::default_random_engine randomEngine(std::random_device{}());
    thread_local std::uniform_int_distribution<int> randomDistribution(0, sizeof(allowed_chars) - 1);
    std::string str_pass(length ? length : 16, '\0');

    for (std::string::value_type& c : str_pass)
        c = allowed_chars[randomDistribution(randomEngine)];

    std::string hashed_pass = sha256(str_pass);
    std::cout << "Pass: " << str_pass << std::endl;

    // Save password in config file
    std::string input;
    std::ofstream out("config_server");
    out << hashed_pass;
}

void bserver::communicate_with_client(char *password, int port, char *key_path) {
    // If pass is not correct -> end
    if(!is_server_password_valid(password)) {
        std::cout << "Given password is not correct. Aborting..." << std::endl;
        return;
    }

    // Load proper private key (N, d)
    read_key_from_file(key_path);

    // Start server
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the given port
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

    // Get message from client
    read(new_socket, buffer, BUFFER_SIZE);
    BIGNUM *m = BN_new();
    BN_hex2bn(&m, buffer);
    std::cout << "Message received from the client: " << BN_bn2hex(m) << std::endl << std::endl;

    // If x is not in Zn group -> abort
    if(!is_msg_in_group(m)) {
        std::cout << "Message x is not in Zn. Aborting..." << std::endl << std::endl;
        return;
    }
    std::cout << "Message x is in Zn." << std::endl << std::endl;

    // Send signed message to client
    char* signed_msg = sign_msg(m);
    std::cout << "Signing..." << std::endl << std::endl;
    send(new_socket, signed_msg, strlen(signed_msg), 0);
    std::cout << "Signed msg sent to client: " << signed_msg << std::endl << std::endl;
    BN_free(m);
}

bool bserver::is_msg_in_group(BIGNUM *num) {
    // If num is in group -> gcd(num,N) == 1
    BIGNUM *gcd = BN_new();
    BIGNUM *one = BN_new();
    BN_gcd(gcd, num, N, ctx);
    int ret = BN_cmp(one, gcd);
    BN_free(gcd);
    BN_free(one);

    return ret != 0;
}

bool bserver::is_server_password_valid(char *user_pass) {
    // Load server's password from config
    std::string line;
    std::ifstream myfile ("config_server");
    if (myfile.is_open()) {
        while (std::getline (myfile, line));
        myfile.close();
    }
    // Hash pass given by user
    std::string user_hashed = sha256(user_pass);
    return line.compare(user_hashed) == 0;
}

char* bserver::sign_msg(BIGNUM *msg_to_sign) {
    // s'= (m')^d (mod N)

    // Measure time
    auto start = std::chrono::high_resolution_clock::now();

    BIGNUM *result = BN_new();
    BN_mod_exp(result, msg_to_sign, d, N, ctx);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Signing time: ";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << std::endl;

    char *ret = BN_bn2hex(result);
    BN_free(result);
    return ret;
}

void bserver::read_key_from_file(char *path) {
    std::cout << "Loading key from: " << path << std::endl;
    std::string item_name;
    std::ifstream nameFileout;
    nameFileout.open(path);
    std::string line;

    //TODO free 'temp' ?
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

//test for PKCS5_PBKDF2_HMAC
void test() {
    const unsigned int KEYLEN = 30;
    const unsigned int SALTLEN = 30;
    const unsigned char salt[SALTLEN] = "SDDSGE23455FDAASDSGE23455FDAG";
    unsigned char out[KEYLEN];
    int iter = 5000, keylen = 30;
    char *password = "password";
    memset(out, 0, sizeof out);

    int r = PKCS5_PBKDF2_HMAC(password, KEYLEN, salt, SALTLEN, iter, EVP_sha256(), keylen, out);

    std::cout << "Generated hash: " << out << std::endl;

    std::string input;
    std::ofstream output("config_server");
    output << salt;
    output << std::endl;
    output << out;
}

bserver::~bserver() {
    RSA_free(r);
    BN_free(num);
    BN_free(N);
    BN_free(d);
    BN_CTX_free(ctx);
}

int main(int argc, char*argv[]) {
//    test();
    if(argc < 3) {
        std::cout << "Missing arguments. Aborting..." << std::endl;
        return -1;
    }

    bserver *server = new bserver();

    if(strcmp(argv[1], "setup") == 0) {
        std::cout << "Setup mode started" << std::endl;
        server->setup(argv[2]);
        return 0;
    }

    if(argc < 5) {
        std::cout << "Missing arguments" << std::endl;
        return -1;
    }

    if(strcmp(argv[1], "sign") == 0) {
        std::cout << "Sign mode started\n" << std::endl;
        server->communicate_with_client(argv[2], atoi(argv[3]), argv[4]);
    }
    else {
        std::cout << "Wrong mode selected. Choose 'setup' or 'sign'" << std::endl;
    }

    return 0;
}