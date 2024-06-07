#include <iostream>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp>

using namespace boost::multiprecision;
using namespace boost::random;

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

// 将字符串转换为大整数
cpp_int string_to_cpp_int(const std::string &str) {
    cpp_int result = 0;
    for (char c : str) {
        result = result * 100 + static_cast<int>(c);
    }
    return result;
}

// 将大整数转换回字符串
std::string cpp_int_to_string(cpp_int num) {
    std::string result;
    while (num > 0) {
        char c = static_cast<char>(num % 100);
        result = c + result;
        num /= 100;
    }
    return result;
}

int main() {
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
