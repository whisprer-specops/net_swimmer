#include <iostream>
#include <string>
#include "chirp_encoder.h"
#include "chirp_decoder.h"
#include "tor_client.h"

void print_usage() {
    std::cout << "Usage:\n"
              << "  chirp_bot --encode <input.json> <chirp.wav>\n"
              << "  chirp_bot --decode <chirp.wav> <output.json>\n"
              << "  chirp_bot --send <input.json>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string command = argv[1];

    if (command == "--encode" && argc == 4) {
        std::string input_file = argv[2];
        std::string output_wav = argv[3];
        if (encode_to_chirp(input_file, output_wav)) {
            std::cout << "[+] Chirp encoded successfully.\n";
        } else {
            std::cerr << "[!] Encoding failed.\n";
            return 1;
        }
    } else if (command == "--decode" && argc == 4) {
        std::string input_wav = argv[2];
        std::string output_file = argv[3];
        if (decode_from_chirp(input_wav, output_file)) {
            std::cout << "[+] Chirp decoded successfully.\n";
        } else {
            std::cerr << "[!] Decoding failed.\n";
            return 1;
        }
    } else if (command == "--send" && argc == 3) {
        std::string input_file = argv[2];
        if (send_blob_over_tor(input_file)) {
            std::cout << "[+] Payload sent over TOR.\n";
        } else {
            std::cerr << "[!] Sending via TOR failed.\n";
            return 1;
        }
    } else {
        print_usage();
        return 1;
    }

    return 0;
}
