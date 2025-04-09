#pragma once
#include <string>
#include <unordered_map>

class Request {
public:
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    static Request parse(const std::string& raw);
};