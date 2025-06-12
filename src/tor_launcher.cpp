#include "tor_launcher.h"
#include "util.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <random>
#include <filesystem>
#include <iostream>

bool fileEmptyOrMissing(const std::string& path) {
    std::ifstream f(path);
    return !f.good() || f.peek() == std::ifstream::traits_type::eof();
}

std::string pickRandomProxy(const std::string& filepath) {
    std::ifstream f(filepath);
    std::vector<std::string> proxies;
    std::string line;
    while (std::getline(f, line)) {
        if (!line.empty()) proxies.push_back(line);
    }
    if (proxies.empty()) return "";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, proxies.size() - 1);
    return proxies[dis(gen)];
}

bool writeTorrcWithUpstreamProxy(const std::string& proxy) {
    std::ofstream torrc("tor/torrc");
    if (!torrc) return false;

    torrc << "SocksPort 9050\n";
    torrc << "ControlPort 9051\n";
    torrc << "CookieAuthentication 0\n";
    torrc << "Log notice stdout\n";
    if (!proxy.empty()) {
        torrc << "Socks5Proxy " << proxy << "\n";
    }

    torrc.close();
    return true;
}

bool launchTor() {
    std::string proxyFile = "proxy/working_proxies.txt";

    if (fileEmptyOrMissing(proxyFile)) {
        std::cout << "[tor_launcher] 🔍 No working proxies found — running scanner...\n";
        system("python proxy/rapidproxyscanner-v-0-4-0.py --save proxy/working_proxies.txt");
    }

    std::string chosenProxy = pickRandomProxy(proxyFile);
    std::cout << "[tor_launcher] 🌐 Selected proxy: " << chosenProxy << "\n";

    if (!writeTorrcWithUpstreamProxy(chosenProxy)) {
        std::cerr << "[tor_launcher] ❌ Failed to write torrc\n";
        return false;
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    std::string command = "tor\\tor.exe -f tor\\torrc";
    if (!CreateProcessA(NULL, command.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "[tor_launcher] ❌ Failed to launch tor.exe\n";
        return false;
    }

    std::cout << "[tor_launcher] 🧅 Tor launched, waiting 10s to bootstrap...\n";
    Sleep(10000); // wait 10s
    return true;
}