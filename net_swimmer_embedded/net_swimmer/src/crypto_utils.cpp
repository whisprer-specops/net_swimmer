#include "crypto_utils.h"
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cctype>

static std::string xor_encrypt(const std::string& data, const std::string& key) {
    std::string output = data;
    for (size_t i = 0; i < data.size(); ++i) {
        output[i] ^= key[i % key.size()];
    }
    return output;
}

static std::string base64_encode(const std::string& input) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output;
    int val=0, valb=-6;
    for (unsigned char c : input) {
        val = (val<<8) + c;
        valb += 8;
        while (valb >= 0) {
            output.push_back(table[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) output.push_back(table[((val<<8)>>(valb+8))&0x3F]);
    while (output.size() % 4) output.push_back('=');
    return output;
}

static std::string base64_decode(const std::string& input) {
    static const char reverse_table[256] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1, 0,-1,-1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1
    };
    std::string output;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    int val = 0, valb = -8;
    for (unsigned char c : input) {
        if (isspace(c) || c == '=') continue;
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            output.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return output;
}

std::string encrypt_blob(const std::string& plaintext, const std::string& key) {
    std::string xored = xor_encrypt(plaintext, key);
    return base64_encode(xored);
}

std::string decrypt_blob(const std::string& b64_ciphertext, const std::string& key) {
    std::string decoded = base64_decode(b64_ciphertext);
    return xor_encrypt(decoded, key);
}