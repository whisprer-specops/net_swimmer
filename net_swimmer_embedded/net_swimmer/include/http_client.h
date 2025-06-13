#pragma once
#include <string>

bool http_post(const std::wstring& server, const std::wstring& endpoint, const std::string& payload);
std::string http_get(const std::wstring& server, const std::wstring& endpoint);
