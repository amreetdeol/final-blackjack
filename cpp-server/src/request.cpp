#include "request.hpp"
#include <sstream>

Request Request::parse(const std::string& raw) {
    Request req;
    std::istringstream stream(raw);
    std::string line;

    std::getline(stream, line);
    std::istringstream first_line(line);
    first_line >> req.method >> req.path;

    while (std::getline(stream, line) && line != "\r") {
        size_t colon = line.find(":");
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            req.headers[key] = value;
        }
    }

    std::getline(stream, req.body, '\0');
    return req;
}