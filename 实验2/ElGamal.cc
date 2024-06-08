#include <iostream>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include "../实验1/AES.h"

using namespace boost::multiprecision;
using namespace boost::random;

// 生成随机素数
static cpp_int generate_prime(int bits, mt19937 &rng) {
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

// 将字符串转换为大整数
static cpp_int string_to_cpp_int(const std::string &str) {
    cpp_int result = 0;
    for (char c : str) {
        result = result * 100 + static_cast<int>(c);
    }
    return result;
}

// 将大整数转换回字符串
static std::string cpp_int_to_string(cpp_int num) {
    std::string result;
    while (num > 0) {
        char c = static_cast<char>(num % 100);
        result = c + result;
        num /= 100;
    }
    return result;
}

// 填充函数
static void add_padding(std::vector<unsigned char>& vec, size_t block_size) {
    size_t padding_len = block_size - (vec.size() % block_size);
    vec.insert(vec.end(), padding_len, static_cast<unsigned char>(padding_len));
}

// 去除填充函数
static void remove_padding(std::vector<unsigned char>& vec) {
    if (vec.empty()) {
        throw std::runtime_error("Invalid padding");
    }
    unsigned char padding_len = vec.back();
    if (padding_len > vec.size()) {
        throw std::runtime_error("Invalid padding");
    }
    vec.resize(vec.size() - padding_len);
}

// 将 cpp_int 转换为 std::vector<unsigned char>
std::vector<unsigned char> cpp_int_to_vector(const cpp_int& num) {
    std::vector<unsigned char> vec;
    export_bits(num, std::back_inserter(vec), 8);

    // 添加填充
    size_t block_size = 16;
    add_padding(vec, block_size);

    return vec;
}

// 将 std::vector<unsigned char> 转换为 cpp_int
cpp_int vector_to_cpp_int(std::vector<unsigned char>& vec) {
    // 去除填充
    remove_padding(vec);

    cpp_int num;
    import_bits(num, vec.begin(), vec.end());
    return num;
}

int test2_2() {
    using namespace boost::multiprecision;

    mt19937 rng(std::random_device{}());

    // 生成大素数 p
    cpp_int p = generate_prime(200, rng);
    cpp_int g = 2;  // 选择生成元 g

    // 选择私钥 x
    cpp_int x = 0;
    while (x == 0 || x >= p-1) {
        x = cpp_int(rng()) % p;
    }

    // 计算公钥 h
    cpp_int h = powm(g, x, p);

    // 打印密钥对
    std::cout << "Public Key: (p = " << p << ", g = " << g << ", h = " << h << ")" << std::endl;
    std::cout << "Private Key: x = " << x << std::endl;

    // 待加密消息
//     std::string message = "26224012ZFY"; // 学号和姓名
    std::string message = "26224011LYT"; // 学号和姓名
    cpp_int message_num = string_to_cpp_int(message);

    // 加密
    cpp_int k = 0;
    while (k == 0 || k >= p-1) {
        k = cpp_int(rng()) % p;
    }
    cpp_int c1 = powm(g, k, p);
    cpp_int c2 = (message_num * powm(h, k, p)) % p;
    std::cout << "Encrypted Message: (" << c1 << ", " << c2 << ")" << std::endl;

    // 解密
    cpp_int s = powm(c1, x, p);
    cpp_int s_inv = powm(s, p-2, p); // 计算 s 的模逆
    cpp_int decrypted = (c2 * s_inv) % p;
    std::string decrypted_message = cpp_int_to_string(decrypted);
    std::cout << "Decrypted Message: " << decrypted_message << std::endl;

    return 0;
}

std::vector<unsigned char> key = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x7f, 0x67, 0x6f, 0xad, 0x74, 0x85
};

std::vector<unsigned char> iv = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

int test2_3() {
    mt19937 rng(std::random_device{}());

    // 生成大素数 p
    cpp_int p = generate_prime(200, rng);
    cpp_int g = 2;  // 选择生成元 g

    // 选择私钥 x
    cpp_int x = 0;
    while (x == 0 || x >= p-1) {
        x = cpp_int(rng()) % p;
    }

    // 计算公钥 h
    cpp_int h = powm(g, x, p);

    // 打印密钥对
    std::cout << "Public Key: (p = " << p << ", g = " << g << ", h = " << h << ")" << std::endl;
    std::cout << "Private Key: x = " << x << std::endl;

    // 待加密消息
//     std::string message = "26224012ZFY"; // 学号和姓名
    std::string message = "26224011LYT"; // 学号和姓名
    cpp_int message_num = string_to_cpp_int(message);

    // 加密
    // ElGamal 加密
    cpp_int k = 0;
    while (k == 0 || k >= p-1) {
        k = cpp_int(rng()) % p;
    }
    cpp_int c1 = powm(g, k, p);
    cpp_int c2 = (message_num * powm(h, k, p)) % p;
    std::cout << "Encrypted Message: (" << c1 << ", " << c2 << ")" << std::endl;

    // AES 加密
    AES aes(AESKeyLength::AES_128);

    // 转换格式
    std::vector<unsigned char> plaintext_c1 = cpp_int_to_vector(c1);
    std::vector<unsigned char> ciphertext_c1 = aes.EncryptCBC(plaintext_c1, key, iv);
    std::vector<unsigned char> plaintext_c2 = cpp_int_to_vector(c2);
    std::vector<unsigned char> ciphertext_c2 = aes.EncryptCBC(plaintext_c2, key, iv);
    std::cout << "Ciphertext(C1) (CBC): ";
    printHex(ciphertext_c1);
    std::cout << "Ciphertext(C2) (CBC): ";
    printHex(ciphertext_c2);

    // 解密
    // AES解密
    std::vector<unsigned char> decryptedtext_c1 = aes.DecryptCBC(ciphertext_c1, key, iv);
    std::vector<unsigned char> decryptedtext_c2 = aes.DecryptCBC(ciphertext_c2, key, iv);

    std::cout << "Decrypted(C1) text (CBC): ";
    printHex(decryptedtext_c1);
    std::cout << "Decrypted(C2) text (CBC): ";
    printHex(decryptedtext_c2);

    // 转换格式
    c1 = vector_to_cpp_int(decryptedtext_c1);
    c2 = vector_to_cpp_int(decryptedtext_c2);

    // ElGamal 解密
    cpp_int s = powm(c1, x, p);
    cpp_int s_inv = powm(s, p-2, p); // 计算 s 的模逆
    cpp_int decrypted = (c2 * s_inv) % p;
    std::string decrypted_message = cpp_int_to_string(decrypted);
    std::cout << "Decrypted Message: " << decrypted_message << std::endl;

    return 0;
}