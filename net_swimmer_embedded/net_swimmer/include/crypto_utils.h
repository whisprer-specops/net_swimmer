#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <string>

std::string encrypt_blob(const std::string& plaintext, const std::string& key);
std::string decrypt_blob(const std::string& b64_ciphertext, const std::string& key);

#endif // CRYPTO_UTILS_H