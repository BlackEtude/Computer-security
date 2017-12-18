//
// Created by pandemic on 17.12.17.
//


#include <random>
#include "sign_test.h"

sign_test::sign_test() {
    ctx = BN_CTX_new();
    N = BN_new();
    d = BN_new();
    r = BN_new();
}

void sign_test::read_key_from_file(char *path) {
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

    int len = BN_num_bytes(d);
    unsigned char* buf = (unsigned char*)malloc(len);
    len = BN_bn2bin(d, buf);

    std::cout << "Len: " << len << std::endl;
}

void sign_test::load_publickey_from_file(char *path) {
    std::cout << "Loading key from: " << path << std::endl << std::endl;
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

double sign_test::sign_msg(BIGNUM *msg_to_sign) {
    // s'= (m')^d (mod N)

    double startsecs, endsecs, seconds;
    uint64_t cycles;

    startsecs = cpu_seconds();
    cycles = local_benchmark(msg_to_sign, d, N);
    endsecs = cpu_seconds();
    seconds = endsecs - startsecs;

//    printf("\t%15.0f CPU cycles\n", (double)cycles);
//    printf("\t%15.0f CPU cycles per second\n", (double)cycles / seconds);
//    printf("\t( %13.3f GHz)\n", (double)cycles / seconds / 1000000000.0);
//    printf("\t%15.9f seconds\n", seconds);
//    printf("-----------------------------------------------------------\n\n");

    return (double)cycles;
}

double sign_test::message_exp(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus) {
    double startsecs, endsecs, seconds;
    uint64_t cycles;

    startsecs = cpu_seconds();
    cycles =  local_benchmark(msg, expo, modulus);
    endsecs = cpu_seconds();
    seconds = endsecs - startsecs;

//    printf("\t%15.0f CPU cycles\n", (double)cycles);
//    printf("\t%15.0f CPU cycles per second\n", (double)cycles / seconds);
//    printf("\t( %13.3f GHz)\n", (double)cycles / seconds / 1000000000.0);
//    printf("\t%15.9f seconds\n", seconds);

    return (double)cycles;
}

void sign_test::test() {
    BIGNUM *private_key = BN_new();
    BIGNUM *z = BN_new();
    BIGNUM *o = BN_new();
    BIGNUM *power_2 = BN_new();
    BIGNUM *two = BN_new();

    const int iters = 500000;

    // ....1
    BN_set_word(private_key, 1);
    // 2^1 (second bit)
    BN_set_word(power_2, 2);
    // to calculate next powers of 2
    BN_set_word(two, 2);

    for(int i = 2; i < 21; i++){
        // New possible exponents: 1...1, 1...0
        BN_copy(z, private_key);
        BN_add(o, private_key, power_2);
        std::cout << "Zero: " << BN_bn2dec(z) << ", one: " << BN_bn2dec(o) << std::endl;

        double sum = 0;
        std::cout << "Bit nr "<< i << std::endl;

        std::vector<double> zeros;
        std::vector<double> ones;

        for(int j = 0; j < iters; j++) {

            // Prepare new message
            BIGNUM *rand_msg = calculate_msg();

            // For 1...0
            //Local expo
            double server_time = sign_msg(rand_msg);
            double local_time0 = message_exp(rand_msg, z, N);
            double diff0 = server_time - local_time0;

            // For 1...1
            //Local expo
            double local_time1 = message_exp(rand_msg, o, N);
            double diff1 = server_time - local_time1;

            zeros.push_back(abs(diff0));
            ones.push_back(abs(diff1));
        }


        double var0 = array_average(zeros);
        double var1 = array_average(ones);

        printf("Zero var: %e\n", var0);
        printf("Ones var: %e\n", var1);

        // Analize result and choose new exponent
        //If more 'ones' than 'zeros'
        if(var1 < var0) {
            BN_add(private_key, private_key, power_2);
            std::cout << "VAR1 < VAR0" << std::endl;
        }
        else
            std::cout << "VAR0 < VAR1" << std::endl;

        std::cout << "New key: " << BN_bn2dec(private_key) << std::endl;
        BN_mul(power_2, power_2, two, ctx);
        std::cout << "----------------------------------------" << std::endl;
    }
}

std::string sign_test::sha256(const std::string str) {
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

BIGNUM* sign_test::calculate_msg() {
    // m' = hash(m) * r^e (mod N)
    int length = rand() % 10 + 3;

    //Generate new msg
    static const std::string::value_type allowed_chars[] {"123456789BCDFGHJKLMNPQRSTVWXZbcdfghjklmnpqrstvwxz"};
    static thread_local std::default_random_engine randomEngine(std::random_device{}());
    static thread_local std::uniform_int_distribution<int> randomDistribution(0, sizeof(allowed_chars) - 1);
    std::string id(length, '\0');
    for (std::string::value_type& c : id) {
        c = allowed_chars[randomDistribution(randomEngine)];
    }

    const char *message = id.c_str();

    std::string hashed_msg = sha256(message);
    const char *hashed_msg_char = hashed_msg.c_str();
    BIGNUM *m = BN_new();
    BN_hex2bn(&m, hashed_msg_char);
    const char* hashed;
    hashed = BN_bn2dec(m);

    BIGNUM *one = BN_new();
    BIGNUM *gcd = BN_new();
    BN_one(one);

    do {
        BN_rand_range(r, N);
        BN_gcd(gcd,r, N, ctx);
    }
    while(BN_cmp(gcd, one) != 0);

    BIGNUM *x = BN_new();
    BN_mod_exp(x, r, e, N, ctx);
    BN_mod_mul(x, m, x, N, ctx);

    BN_free(one);
    BN_free(gcd);
    BN_free(m);
    return x;
}

double sign_test::array_average(std::vector<double> array) {
    unsigned long length = array.size();
    double avg = 0;
    for(unsigned long i = 0; i < array.size(); i++) {
        avg += array.at(i);
    }

    avg = avg/length;

    double var = 0;
    for(unsigned long i = 0; i < array.size(); i++)
        var += (array.at(i) - avg) * (array.at(i) - avg);

    var /= (length);
    return var;

}

BIGNUM* sign_test::modular_exponentation(BIGNUM *x, BIGNUM *y, BIGNUM *m) {
    BIGNUM *result = BN_new();
    BN_set_word(result, 1);

//    x = x % m;
    BIGNUM *zero = BN_new();

    while(BN_cmp(y, zero) == 1) {                   // y > 0
        //y is odd -> mul x with result
        if(BN_is_odd(y) == 1)
            BN_mod_mul(result, result, x, m, ctx);  // result = (result*x) % m;
        //y must be even now => div by 2
        BN_rshift1(y, y);                           // y = y >> 1;
        BN_mod_mul(x, x, x, m, ctx);                // x = (x*x) % m;
    }
    return result;
}

uint64_t sign_test::local_benchmark(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus) {
    uint64_t started, finished;
    BIGNUM *result = BN_new();

    started = rdtsc();
    modular_exponentation(msg, expo, modulus);
    finished = rdtsc();

    return finished - started;
}

double sign_test::cpu_seconds() {
    struct timespec t;
    if (!clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t))
        return (double)t.tv_sec + (double)t.tv_nsec / 1000000000.0;
    else
        return (double)clock() / (double)CLOCKS_PER_SEC;
}

uint64_t sign_test::rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ( ((uint64_t)hi) << (uint32_t)32 )
           | ( ((uint64_t)lo) );
}

int main(int argc, char* argv[]) {

    //start: ./test key/private2048 key/public2048
    if(argc < 3) {
        std::cout << "Missing arguments. Aborting..." << std::endl;
        return -1;
    }

    auto *test = new sign_test();
    // Load proper private key (N, d)
    test->read_key_from_file(argv[1]);

    // Load proper public key (N, e)
    test->load_publickey_from_file(argv[2]);

    test->test();

    return 0;
}
