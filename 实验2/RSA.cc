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

// 计算模逆
cpp_int mod_inverse(cpp_int a, cpp_int m) {
    cpp_int m0 = m, t, q;
    cpp_int x0 = 0, x1 = 1;
    if (m == 1)
        return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;
    return x1;
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

    mt19937 rng(std::random_device{}());

    // 生成大素数 p 和 q
    cpp_int p = generate_prime(200, rng);
    cpp_int q = generate_prime(200, rng);
    
    // 计算 n 和 φ(n)
    cpp_int n = p * q;
    cpp_int phi_n = (p - 1) * (q - 1);

    // 选择公钥指数 e
    cpp_int e = 65537;  // 通常选择这个值

    // 计算私钥指数 d
    cpp_int d = mod_inverse(e, phi_n);

    // 打印密钥对
    std::cout << "Public Key: (n = " << n << ", e = " << e << ")" << std::endl;
    std::cout << "Private Key: (n = " << n << ", d = " << d << ")" << std::endl;

    // 待加密消息
//     std::string message = "26224011LYT"; // 学号和姓名
    std::string message = "26224012ZFY"; // 学号和姓名
    cpp_int message_num = string_to_cpp_int(message);

    // 加密
    cpp_int encrypted = powm(message_num, e, n);
    std::cout << "Encrypted Message: " << encrypted << std::endl;

    // 解密
    cpp_int decrypted = powm(encrypted, d, n);
    std::string decrypted_message = cpp_int_to_string(decrypted);
    std::cout << "Decrypted Message: " << decrypted_message << std::endl;

    return 0;
}
