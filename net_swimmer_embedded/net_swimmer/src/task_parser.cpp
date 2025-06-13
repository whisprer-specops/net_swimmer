
#include "task_parser.h"
#include <string>
#include <regex>

// Very simple regex-based JSON logic extractor
std::string extract_logic(const std::string& json_blob) {
    std::smatch match;
    std::regex logic_pattern(R"(\"logic\"\s*:\s*\"(.*?)\")");
    if (std::regex_search(json_blob, match, logic_pattern)) {
        return match[1].str();
    }
    return "";
}

std::string extract_task(const std::string& json_blob) {
    std::smatch match;
    std::regex task_pattern(R"(\"task\"\s*:\s*\"(.*?)\")");
    if (std::regex_search(json_blob, match, task_pattern)) {
        return match[1].str();
    }
    return "default";
}
