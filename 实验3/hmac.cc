#include "hmac.h"

// 将字节数组转换为十六进制字符串
std::string to_hex_string(const unsigned char* bytes, size_t length) {
    std::string hex_str;
    for (size_t i = 0; i < length; ++i) {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", bytes[i]);
        hex_str.append(buf);
    }
    return hex_str;
}

// 使用 SHA-256 生成 HMAC
std::string generate_hmac(const std::string& key, const std::string& message) {
    unsigned char* digest;
    digest = HMAC(EVP_sha256(), key.c_str(), key.length(), 
                  (unsigned char*)message.c_str(), message.length(), NULL, NULL);
    
    return to_hex_string(digest, SHA256_DIGEST_LENGTH);
}

// 验证HMAC
bool verify_hmac(const std::string& key, const std::string& message, const std::string& mac) {
    std::string calculated_mac = generate_hmac(key, message);
    return calculated_mac == mac;
}

void test3_1() {
    std::string key = "secret_key"; // HMAC 密钥
    std::string message = "26224012ZFY"; // 学号+姓名
    
    // 为消息生成 MAC
    std::string mac = generate_hmac(key, message);
    std::cout << "Generated MAC: " << mac << std::endl;
    
    // 验证MAC
    if (verify_hmac(key, message, mac)) {
        std::cout << "MAC verification succeeded. Message is authentic and intact." << std::endl;
    } else {
        std::cout << "MAC verification failed. Message is not authentic or has been tampered with." << std::endl;
    }
}
