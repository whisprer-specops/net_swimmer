
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

// Placeholder: assumes this sends the payload via TOR
bool send_via_tor(const std::string& json_payload) {
    std::cout << "[*] Sending payload via TOR:\n" << json_payload << std::endl;
    return true;
}

bool starts_with(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

bool ends_with(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Generate a numbered filename like js_data_0001.json
std::string generate_output_filename(const std::string& directory) {
    int max_index = 0;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::string name = entry.path().filename().string();
        if (starts_with(name, "js_data_") && ends_with(name, ".json")) {
            try {
                int num = std::stoi(name.substr(8, name.size() - 13));
                if (num > max_index) max_index = num;
            } catch (...) {}
        }
    }
    std::ostringstream oss;
    oss << directory << "/js_data_" << std::setw(4) << std::setfill('0') << (max_index + 1) << ".json";
    return oss.str();
}

void handle_js_payload(const std::string& json_data) {
    std::string output_dir = "data";
    std::filesystem::create_directories(output_dir);

    std::string filename = generate_output_filename(output_dir);
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "[!] Failed to write JS data to " << filename << std::endl;
        return;
    }

    outfile << json_data;
    outfile.close();

    std::cout << "[+] JS data written to " << filename << std::endl;

    if (send_via_tor(json_data)) {
        std::cout << "[+] Payload sent successfully via TOR!" << std::endl;
    } else {
        std::cerr << "[!] TOR transmission failed!" << std::endl;
    }
}
