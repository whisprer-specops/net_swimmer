#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

extern bool sendJsonViaTor(const std::string& jsonPayload);

std::string generateReportJson() {
    std::time_t now = std::time(nullptr);
    char timeStr[64];
    std::strftime(timeStr, sizeof(timeStr), "%FT%TZ", std::gmtime(&now));

    std::string report = R"({
        "agent": "net_swimmer",
        "status": "online",
        "timestamp": ")";
    report += timeStr;
    report += R"("})";

    return report;
}

int main() {
    std::string payload = generateReportJson();
    std::cout << "[*] Sending payload:\n" << payload << "\n";

    bool success = sendJsonViaTor(payload);
    if (success) {
        std::cout << "[+] Report sent successfully via TOR!\n";
    } else {
        std::cerr << "[-] Failed to send report.\n";
    }

    return success ? 0 : 1;
}
