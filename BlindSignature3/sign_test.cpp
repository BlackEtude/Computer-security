//
// Created by pandemic on 17.12.17.
//

#include "sign_test.h"
#define ITERS 300000

sign_test::sign_test(char *priv_path, char *pub_path) {
    ctx = BN_CTX_new();
    N = BN_new();
    d = BN_new();
    e = BN_new();
    r = BN_new();

    // Load proper keys (N, d), (N, e)
    read_key_from_file(priv_path, N, d);
    read_key_from_file(pub_path, N, e);

    // Run attack!
    test();
}

void sign_test::read_key_from_file(char *path, BIGNUM *x, BIGNUM *y) {
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
    BN_hex2bn(&x, c);
    c = temp[1].c_str();
    BN_hex2bn(&y, c);
}

void sign_test::test() {
    std::cout << "d: " << BN_bn2dec(d) << std::endl;
    BIGNUM *private_key = BN_new();
    BIGNUM *z = BN_new();
    BIGNUM *o = BN_new();
    BIGNUM *power_2 = BN_new();
    BIGNUM *two = BN_new();
    std::string final_key = "1";

    // To count failed  attemps
    int fails = 0;
    
    std::vector<double> zeros;
    std::vector<double> ones;

    // Start: 1
    BN_set_word(private_key, 1);
    // 2^1 (second bit)
    BN_set_word(power_2, 2);
    // to calculate next powers of 2
    BN_set_word(two, 2);

    double server_time, local_time, diff;
    double var0, var1;

    for(int i = 2; i < 18; i++) {
        // New possible exponents: 0...1, 1...1
        BN_copy(z, private_key);
        BN_add(o, private_key, power_2);
        std::cout << "Bit no "<< i << std::endl;
        // std::cout << "Zero: " << BN_bn2dec(z) << std::endl;
        // std::cout << "One: " << BN_bn2dec(o) << std::endl;

        for(int j = 0; j < ITERS; j++) {
            // Prepare new message
            BIGNUM *rand_msg = generate_msg();

            // Measure time of signing for private key
            server_time = sign_msg(rand_msg, d, N);

            // Measure time for new key 0...1
            local_time = sign_msg(rand_msg, z, N);
            diff = server_time - local_time;
            zeros.push_back(abs(diff));

            // Measure time for new key 1...1
            local_time = sign_msg(rand_msg, o, N);
            diff = server_time - local_time;
            ones.push_back(abs(diff));

            BN_free(rand_msg);
        }

        var0 = array_var(zeros);
        var1 = array_var(ones);

        printf("'Zero' var: %e\n", var0);
        printf("'One' var: %e\n", var1);

        // Analize results and choose new exponent's bit
        // Diff between both vars are enough
        if(!is_proper_diff(var0, var1)) {
            if(fails < 6) {
                i--;
                std::cout << "REPEAT" << std::endl;
                fails++;
                continue;
            }
            else {
                // Get new bit from user
                fails = 0;
                std::cout << "Give me a hint... :(" << std::endl << "Bit: ";
                char bit;
                std::cin >> bit;
                if(bit == '1') {
                    BN_add(private_key, private_key, power_2);
                    final_key = final_key + '1';
                }
                else
                    final_key = final_key + '0';
            }
        }
        // Diff is okey
        else {
            // Set new bit
            if(var1 < var0) {
                BN_add(private_key, private_key, power_2);
                std::cout << "VAR1 < VAR0" << std::endl;
                final_key = final_key + '1';
            }
            else {
                std::cout << "VAR0 < VAR1" << std::endl;
                final_key = final_key + '0';
            }
            fails = 0;
        }        

        // Prepare next power of 2
        BN_CTX_start(ctx);
        BN_mul(power_2, power_2, two, ctx);
        BN_CTX_end(ctx);

        // Print key with padding
        std::cout << "Key: ";
        for(int i = final_key.length()-1; i >= 0; i--) {
            std::cout << final_key[i];
            if(i % 4 == 0)
                std::cout << " ";
        }
        std::cout << std::endl << std::endl << std::endl;

        // Cleanup
        ones.clear();
        ones.shrink_to_fit();
        zeros.clear();
        zeros.shrink_to_fit();
    }

    // Cleanup
    BN_free(private_key);
    BN_free(z);
    BN_free(o);
    BN_free(power_2);
    BN_free(two);
}

bool sign_test::is_proper_diff(double var0, double var1) {
    std::string v0 = std::to_string((int)var0); 
    std::string v1 = std::to_string((int)var1); 

    // std::cout << "Var0: " << v0 << ", var1: " << v1 << std::endl;
    if(v0.length() == v1.length()) 
        return false;
    else
        return true;
}

BIGNUM* sign_test::generate_msg() {
    // m' = hash(m) * r^e (mod N)
    int length = rand() % 20 + 8;

    // Generate new msg
    static const std::string::value_type allowed_chars[] {"123456789BCDFGHJKLMNPQRSTVWXZbcdfghjklmnpqrstvwxz"};
    static thread_local std::default_random_engine randomEngine(std::random_device{}());
    static thread_local std::uniform_int_distribution<int> randomDistribution(0, sizeof(allowed_chars) - 1);
    std::string id(length, '\0');
    for (std::string::value_type& c : id)
        c = allowed_chars[randomDistribution(randomEngine)];

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
    BN_CTX_start(ctx);

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
    BN_CTX_end(ctx);
    return x;
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

double sign_test::sign_msg(BIGNUM *msg_to_sign, BIGNUM *expo, BIGNUM *modulus) {
    // s'= (m')^d (mod N)
    double startsecs, endsecs, seconds;
    uint64_t cycles;

    startsecs = cpu_seconds();
    cycles = benchmark(msg_to_sign, expo, modulus);
    endsecs = cpu_seconds();
    seconds = endsecs - startsecs;

//    printf("\t%15.0f CPU cycles\n", (double)cycles);
//    printf("\t%15.0f CPU cycles per second\n", (double)cycles / seconds);
//    printf("\t( %13.3f GHz)\n", (double)cycles / seconds / 1000000000.0);
//    printf("\t%15.9f seconds\n", seconds);
//    printf("-----------------------------------------------------------\n\n");

    return (double)cycles;
}

uint64_t sign_test::benchmark(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus) {
    uint64_t started, finished;

    started = rdtsc();
    modular_exponentation(msg, expo, modulus);
    finished = rdtsc();

    return finished - started;
}

uint64_t sign_test::rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtscp" : "=a" (lo), "=d" (hi));
    return ( ((uint64_t)hi) << (uint32_t)32 )
           | ( ((uint64_t)lo) );
}

void sign_test::modular_exponentation(BIGNUM *x, BIGNUM *y, BIGNUM *m) {
    BIGNUM *result = BN_new();
    BIGNUM *zero = BN_new();                        
    BN_set_word(result, 1);
    BN_set_word(zero, 0);


    //    x = x % m;
    while(BN_cmp(y, zero) == 1) {                   // y > 0
        //y is odd -> mul x with result
        if(BN_is_odd(y) == 1) {
            BN_CTX_start(ctx);
            BN_mod_mul(result, result, x, m, ctx);  // result = (result*x) % m;
            BN_CTX_end(ctx);
        }
        //y must be even now => div by 2
        BN_rshift1(y, y);                           // y = y >> 1;
        BN_CTX_start(ctx);
        BN_mod_mul(x, x, x, m, ctx);                // x = (x*x) % m;
        BN_CTX_end(ctx);
    }
    BN_free(zero);
    BN_free(result);
}

double sign_test::array_var(std::vector<double> array) {
    unsigned long length = array.size();
    double avg = 0;

    for (double i : array)
        avg += i;

    avg = avg/length;

    double var = 0;
    for (double i : array)
        var += (i - avg) * (i - avg);

    var /= (length);
    return var;
}

// Perhaps useless....
double sign_test::cpu_seconds() {
    struct timespec t;
    if (!clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t))
        return (double)t.tv_sec + (double)t.tv_nsec / 1000000000.0;
    else
        return (double)clock() / (double)CLOCKS_PER_SEC;
}

int main(int argc, char* argv[]) {

    //start: ./test key/private2048 key/public2048
    if(argc < 3) {
        std::cout << "Missing arguments. Aborting..." << std::endl;
        return -1;
    }

    auto *test = new sign_test(argv[1], argv[2]);

    return 0;
}
