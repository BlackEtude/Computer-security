//
// Created by pandemic on 17.12.17.
//

#include "bserver_attack.h"
#define ITERS 300000

bserver_attack::bserver_attack(char *priv_path, char *pub_path, int bits) {
    ctx = BN_CTX_new();
    N = BN_new();
    d = BN_new();
    e = BN_new();
    r = BN_new();

    // Load proper keys (N, d), (N, e)
    read_key_from_file(priv_path, N, d);
    read_key_from_file(pub_path, N, e);

    // Run attack!
    test(bits);
}

void bserver_attack::read_key_from_file(char *path, BIGNUM *x, BIGNUM *y) {
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

void bserver_attack::test(int bits) {
    std::cout << "d: " << BN_bn2dec(d) << std::endl;
    BIGNUM *private_key = BN_new();
    BIGNUM *z = BN_new();
    BIGNUM *o = BN_new();
    BIGNUM *power_2 = BN_new();
    BIGNUM *two = BN_new();
    std::string final_key = "1";

    // Failed attempts counter
    int repeats = 0;
    int attempts = 0;
    int sugg_zero = 0;
    int sugg_one = 0;
    char bit;

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

    for(int i = 2; i <= bits; i++) {
        // New possible exponent: 0...1 or 1...1
        BN_copy(z, private_key);
        BN_add(o, private_key, power_2);
        std::cout << "Bit no "<< i << std::endl;

        for(int j = 0; j < ITERS; j++) {
            // Prepare new message
            BIGNUM *rand_msg = generate_msg();

            // Measure time for private key
            server_time = sign_msg(rand_msg, d, N);

            // Measure time for test key 0...1
            local_time = sign_msg(rand_msg, z, N);
            diff = server_time - local_time;
            zeros.push_back(abs(diff));

            // Measure time for test key 1...1
            local_time = sign_msg(rand_msg, o, N);
            diff = server_time - local_time;
            ones.push_back(abs(diff));

            BN_free(rand_msg);
        }

        var0 = array_var(zeros);
        var1 = array_var(ones);

        printf("'Zero' variance: %e\n", var0);
        printf("'One'  variance: %e\n", var1);

        if(var1 < var0) {
            std::cout << "VAR0 > VAR1" << std::endl;
            sugg_one++;
        }
        else {
            std::cout << "VAR0 < VAR1" << std::endl;
            sugg_zero++;
        }

        // Analize results and choose new exponent's bit
        // Diff between both vars is not enough
        if(!diff_is_enough(var0, var1)) {
            if(repeats < 8) {
                std::cout << "REPEAT" << std::endl;
                i--;
                repeats++;
                continue;
            }
            else {
                // Get new bit from user
                repeats = 0;
                std::cout << sugg_zero*100/(sugg_zero+sugg_one) << " % '0' \t";
                std::cout << sugg_one*100/(sugg_zero+sugg_one) << " % '1" << std::endl;
                std::cout << "Give me a hint..." << std::endl << "Bit: ";
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
            if(sugg_zero == sugg_one) {
                i--;
                continue;
                std::cout << "REPEAT - zeros = ones" << std::endl;
            }
            std::cout << "Suggested: " << ((sugg_zero > sugg_one) ? '0': '1') << std::endl;
            std::cout << "'0' : " << sugg_zero << ", '1' : " << sugg_one << std::endl;
            std::cout << "Repeat y/n?" << std::endl;
            std::cin >> bit;
            if(bit == 'y') {
                i--;
                repeats - 0;
                sugg_one = 0;
                sugg_zero = 0;
                continue;
            }
            // Set new bit
            if(sugg_one > sugg_zero) {
                BN_add(private_key, private_key, power_2);
                final_key = final_key + '1';
            }
            else {
                final_key = final_key + '0';
            }
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

        repeats = 0;
        sugg_zero = 0;
        sugg_one = 0;

        // Cleanup
        ones.clear();
        ones.shrink_to_fit();
        zeros.clear();
        zeros.shrink_to_fit();

        sleep(1);
    }

    // Cleanup
    BN_free(private_key);
    BN_free(z);
    BN_free(o);
    BN_free(power_2);
    BN_free(two);
}

bool bserver_attack::diff_is_enough(double var0, double var1) {
    std::string v0 = std::to_string((int)var0); 
    std::string v1 = std::to_string((int)var1); 

    if(v0.length() == v1.length())
        return false;
    else
        return true;
}

double bserver_attack::sign_msg(BIGNUM *msg_to_sign, BIGNUM *expo, BIGNUM *modulus) {
    // s'= (m')^d (mod N)
    double startsecs, endsecs, seconds;
    uint64_t cycles;

    startsecs = cpu_seconds();
    cycles = benchmark(msg_to_sign, expo, modulus);
    endsecs = cpu_seconds();
    seconds = endsecs - startsecs;

    return (double)cycles;
}

uint64_t bserver_attack::benchmark(BIGNUM *msg, BIGNUM *expo, BIGNUM *modulus) {
    uint64_t started, finished;

    started = rdtscp();
    modular_exponentation(msg, expo, modulus);
    finished = rdtscp();

    return finished - started;
}

uint64_t bserver_attack::rdtscp() {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtscp" : "=a" (lo), "=d" (hi));
    return ( ((uint64_t)hi) << (uint32_t)32 )
           | ( ((uint64_t)lo) );
}

void bserver_attack::modular_exponentation(BIGNUM *x, BIGNUM *y, BIGNUM *m) {
    BIGNUM *result = BN_new();
    BIGNUM *zero = BN_new();                        
    BN_set_word(result, 1);
    BN_set_word(zero, 0);

    // x = x % m;
    while(BN_cmp(y, zero) == 1) {                   // y > 0
        // y is odd ->  x * result
        if(BN_is_odd(y) == 1) {
            BN_CTX_start(ctx);
            BN_mod_mul(result, result, x, m, ctx);  // result = (result*x) % m;
            BN_CTX_end(ctx);
        }
        // y must be even now => div by 2
        BN_rshift1(y, y);                           // y = y >> 1;
        BN_CTX_start(ctx);
        BN_mod_mul(x, x, x, m, ctx);                // x = (x*x) % m;
        BN_CTX_end(ctx);
    }
    BN_free(zero);
    BN_free(result);
}

double bserver_attack::array_var(std::vector<double> array) {
    unsigned long length = array.size();
    double avg = 0;

    // sort
    // std::sort(array.begin(), array.end());
    // std::reverse(array.begin(), array.end());

    for (double i : array)
        avg += i;

    avg = avg/length;
    // std::cout << "Average: " << avg << std::endl;

    double var = 0;
    for (double i : array)
        var += (i - avg) * (i - avg);

    var /= (length);
    return var;
}

// Probably useless....
double bserver_attack::cpu_seconds() {
    struct timespec t;
    if (!clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t))
        return (double)t.tv_sec + (double)t.tv_nsec / 1000000000.0;
    else
        return (double)clock() / (double)CLOCKS_PER_SEC;
}

BIGNUM* bserver_attack::generate_msg() {
    // m' = hash(m) * r^e (mod N)
    int length = rand() % 30 + 8;

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

std::string bserver_attack::sha256(const std::string str) {
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

int main(int argc, char* argv[]) {

    //start: ./test key/private2048 key/public2048 2048
    if(argc < 4) {
        std::cout << "Missing arguments. Aborting..." << std::endl;
        return -1;
    }

    new bserver_attack(argv[1], argv[2], atoi(argv[3]));

    return 0;
}
