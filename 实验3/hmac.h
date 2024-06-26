#ifndef __HMAC_H__
#define __HMAC_H__

#include <iostream>
#include <string>
#include <openssl/sha.h>
#include <openssl/hmac.h>

std::string to_hex_string(const unsigned char* bytes, size_t length);
std::string generate_hmac(const std::string& key, const std::string& message);
bool verify_hmac(const std::string& key, const std::string& message, const std::string& mac);
void test3_1();

#endif