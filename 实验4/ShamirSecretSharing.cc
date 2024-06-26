#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random.hpp>

using namespace boost::multiprecision;
using namespace boost::random;

// Lagrange插值法
cpp_int lagrange_interpolation(const std::vector<std::pair<int, cpp_int>>& points, int x) {
    cpp_int result = 0;
    for (size_t i = 0; i < points.size(); i++) {
        cpp_int term = points[i].second;
        for (size_t j = 0; j < points.size(); j++) {
            if (i != j) {
                term *= (x - points[j].first);
                term /= (points[i].first - points[j].first);
            }
        }
        result += term;
    }
    return result;
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

// 分割秘密
std::vector<std::pair<int, cpp_int>> split_secret(cpp_int secret, int n, int t, cpp_int prime, mt19937 &rng) {
    std::vector<cpp_int> coefficients(t);
    coefficients[0] = secret;
    for (int i = 1; i < t; i++) {
        coefficients[i] = cpp_int(rng()) % prime;
    }

    std::vector<std::pair<int, cpp_int>> shares(n);
    for (int i = 0; i < n; i++) {
        cpp_int x = i + 1;
        cpp_int y = 0;
        for (int j = 0; j < t; j++) {
            y += coefficients[j] * cpp_int(pow(x, j));
            y %= prime;
        }
        shares[i] = { static_cast<int>(x), y };
    }
    return shares;
}

void shamir_secret_sharing() {
    // 示例: n = 5, t = 3
    int n = 5;
    int t = 3;
    std::string secret_str = "26224011"; // 只包含数字部分的学号部分
    cpp_int secret;
    try {
        secret = cpp_int(secret_str.c_str());  // 直接使用cpp_int初始化秘密
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: Failed to convert secret string to cpp_int." << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;
        return;
    }

    std::cout << "Secret as cpp_int: " << secret << std::endl;

    // 创建随机数生成器
    mt19937 rng(std::random_device{}());

    // 定义一个大素数
    cpp_int prime = generate_prime(256, rng);

    std::vector<std::pair<int, cpp_int>> shares = split_secret(secret, n, t, prime, rng);

    std::cout << "Shares:" << std::endl;
    for (const auto& share : shares) {
        std::cout << "x: " << share.first << ", y: " << share.second << std::endl;
    }

    // 恢复秘密
    std::vector<std::pair<int, cpp_int>> subset(shares.begin(), shares.begin() + t);
    cpp_int recovered_secret = lagrange_interpolation(subset, 0);

    std::cout << "Recovered secret: " << recovered_secret << std::endl;

    std::string recovered_str = recovered_secret.str();
    std::cout << "Recovered secret string: " << recovered_str << std::endl;
}

int main() {
    shamir_secret_sharing();
    return 0;
}
