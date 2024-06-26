#include <iostream>
#include <string>
#include "../实验1/AES.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

using namespace boost::multiprecision;
using namespace boost::random;

cpp_int mod_exp(cpp_int base, cpp_int exp, cpp_int mod) {
    cpp_int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

bool miller_rabin_test(const cpp_int &n, int k, mt19937 &rng) {
    if (n <= 4)
        return n == 2 || n == 3;
    
    cpp_int d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    std::uniform_int_distribution<uint64_t> dist(2, n.convert_to<uint64_t>() - 2);

    for (int i = 0; i < k; ++i) {
        cpp_int a = dist(rng);
        cpp_int x = mod_exp(a, d, n);
        
        if (x == 1 || x == n - 1)
            continue;
        
        bool continue_outer_loop = false;
        for (int j = 0; j < r - 1; ++j) {
            x = mod_exp(x, 2, n);
            if (x == n - 1) {
                continue_outer_loop = true;
                break;
            }
        }
        
        if (continue_outer_loop)
            continue;
        
        return false;
    }
    
    return true;
}

// 生成随机素数
cpp_int generate_prime(int bits, mt19937 &rng) {
    while (true) {
        cpp_int p = 0;
        for (int i = 0; i < bits; ++i) {
            if (rng() % 2) {
                p |= cpp_int(1) << i;
            }
        }
        p |= cpp_int(1) << (bits - 1);  // 确保是 bits 位数
        if (miller_rabin_test(p, 25, rng)) {
            return p;
        }
    }
}

void dh_key_exchange() {
    mt19937 rng(std::random_device{}());
    std::vector<unsigned char> key = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x7f, 0x67, 0x6f, 0xad, 0x74, 0x85
    };
    std::vector<unsigned char> iv = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    // 生成大素数 p 和生成元 g
    cpp_int p = generate_prime(512, rng);
    cpp_int g = 2;

    // Alice 选择私钥 a 并计算公钥 A
    cpp_int a = cpp_int(rng()) % p;
    cpp_int A = mod_exp(g, a, p);

    // Bob 选择私钥 b 并计算公钥 B
    cpp_int b = cpp_int(rng()) % p;
    cpp_int B = mod_exp(g, b, p);

    // 交换公钥并计算共享密钥
    cpp_int shared_secret_A = mod_exp(B, a, p);
    cpp_int shared_secret_B = mod_exp(A, b, p);

    std::cout << "Alice's Shared Secret: " << shared_secret_A << std::endl;
    std::cout << "Bob's Shared Secret: " << shared_secret_B << std::endl;

    // 检查共享密钥是否相同
    if (shared_secret_A == shared_secret_B) {
        std::cout << "Shared secret keys match!" << std::endl;
    } else {
        std::cout << "Shared secret keys do not match!" << std::endl;
    }

    // 使用共享密钥进行AES加密解密
    std::string message = "26224011LYT";
    AES aes(AESKeyLength::AES_128);

    export_bits(shared_secret_A, key.begin(), 8);

    std::vector<unsigned char> plaintext = string_to_vector(message, 16);


    // 加密
    std::vector<unsigned char> ciphertext = aes.EncryptCBC(plaintext, key, iv);
    std::cout << "Encrypted Message: ";
    for (auto c : ciphertext) {
        std::cout << std::hex << (int)c << " ";
    }
    std::cout << std::endl;
 
    // 解密
    std::vector<unsigned char> decryptedtext = aes.DecryptCBC(ciphertext, key, iv);
    std::string decrypted_message(decryptedtext.begin(), decryptedtext.end());
    std::cout << "Decrypted Message: " << decrypted_message << std::endl;
}

int main() {
    dh_key_exchange();
    return 0;
}
