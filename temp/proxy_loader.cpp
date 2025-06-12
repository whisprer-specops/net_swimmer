
#include "proxy_loader.h"
#include <fstream>
#include <sstream>

std::vector<std::string> loadProxies(const std::string& path) {
    std::vector<std::string> proxies;
    std::ifstream infile(path);
    std::string line;
    while (std::getline(infile, line)) {
        if (!line.empty()) {
            proxies.push_back(line);
        }
    }
    return proxies;
}
