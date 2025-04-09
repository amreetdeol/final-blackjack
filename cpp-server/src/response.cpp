#include "response.hpp"
#include <sstream>

std::string Response::toString() const {
    std::ostringstream res;
    res << "HTTP/1.1 " << status_code << " OK\r\n";
    res << "Content-Type: " << content_type << "\r\n";
    res << "Content-Length: " << body.length() << "\r\n";

    for (const auto& [key, value] : headers) {
        res << key << ": " << value << "\r\n";
    }

    res << "Connection: close\r\n\r\n";
    res << body;
    return res.str();
}