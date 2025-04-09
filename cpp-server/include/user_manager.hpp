#pragma once
#include <unordered_map>
#include <string>
#include <mutex>

class UserManager {
public:
    bool login(const std::string& username, const std::string& password);
    std::string createSession(const std::string& username);
    bool validateSession(const std::string& sessionId);
    std::string getUsernameFromSession(const std::string& sessionId);

    bool registerUser(const std::string& username, const std::string& password);

private:
    std::unordered_map<std::string, std::string> userPasswords;  // username → password
    std::unordered_map<std::string, std::string> sessions;       // sessionId → username
    std::mutex mutex;
};