
#ifndef TASK_PARSER_H
#define TASK_PARSER_H

#include <string>

std::string extract_logic(const std::string& json_blob);
std::string extract_task(const std::string& json_blob);

#endif // TASK_PARSER_H
