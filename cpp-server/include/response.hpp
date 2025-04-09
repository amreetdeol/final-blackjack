#pragma once
#include <string>
#include <unordered_map>

class Response {
public:
    int status_code = 200;
    std::string content_type = "text/plain";
    std::string body;

    std::unordered_map<std::string, std::string> headers;

    std::string toString() const;
};