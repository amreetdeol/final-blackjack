#include "user_manager.hpp"

bool UserManager::registerUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(mutex);
    if (userPasswords.count(username)) {
        return false; 
    }
    userPasswords[username] = password;
    return true;
}

bool UserManager::login(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = userPasswords.find(username);
    return it != userPasswords.end() && it->second == password;
}

std::string UserManager::createSession(const std::string& username) {
    std::lock_guard<std::mutex> lock(mutex);
    std::string token = "session-" + username;
    sessions[token] = username;
    return token;
}

bool UserManager::validateSession(const std::string& sessionId) {
    std::lock_guard<std::mutex> lock(mutex);
    return sessions.find(sessionId) != sessions.end();
}

std::string UserManager::getUsernameFromSession(const std::string& sessionId) {
    std::lock_guard<std::mutex> lock(mutex);
    return sessions.count(sessionId) ? sessions[sessionId] : "";
}