#include "router.hpp"
#include "user_manager.hpp"
#include "game_manager.hpp"
#include "game_table.hpp"

#include <iostream>
#include <algorithm>

namespace {
    UserManager um;
    GameManager& gm = GameManager::getInstance();

    void addCORSHeaders(Response& res) {
        res.headers["Access-Control-Allow-Origin"] = "*";
        res.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization";
        res.headers["Access-Control-Allow-Methods"] = "GET, POST, OPTIONS";
    }

    std::string cleanSession(const std::string& raw) {
        std::string result = raw;
        result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
        return result;
    }

    std::string extractSessionId(const Request& req) {
        auto it = req.headers.find("Authorization");
        return cleanSession(it != req.headers.end() ? it->second : "");
    }

    std::string getValidatedUsername(const std::string& sessionId, Response& res) {
        std::cout << "[Auth] Received session header: '" << sessionId << "'" << std::endl;

        if (!um.validateSession(sessionId)) {
            std::cout << "[Auth] Invalid session" << std::endl;
            res.status_code = 403;
            res.body = "Invalid session";
            addCORSHeaders(res);
            return "";
        }

        std::string username = um.getUsernameFromSession(sessionId);
        std::cout << "[Auth] Valid session. Username: " << username << std::endl;
        return username;
    }

    Response handleRegister(const std::string& body) {
        Response res;
        auto sep = body.find(":");
        if (sep == std::string::npos) {
            res.status_code = 400;
            res.body = "Invalid format. Use username:password";
            addCORSHeaders(res);
            return res;
        }

        std::string username = body.substr(0, sep);
        std::string password = body.substr(sep + 1);

        if (um.registerUser(username, password)) {
            res.status_code = 200;
            res.body = "User registered";
        } else {
            res.status_code = 409;
            res.body = "Username already exists";
        }

        addCORSHeaders(res);
        return res;
    }

    Response handleLogin(const std::string& body) {
        Response res;
        auto sep = body.find(":");
        if (sep == std::string::npos) {
            res.status_code = 400;
            res.body = "Invalid format. Use username:password";
            addCORSHeaders(res);
            return res;
        }

        std::string username = body.substr(0, sep);
        std::string password = body.substr(sep + 1);

        if (um.login(username, password)) {
            std::string session = um.createSession(username);
            res.status_code = 200;
            res.body = session;
        } else {
            res.status_code = 403;
            res.body = "Login failed";
        }

        addCORSHeaders(res);
        return res;
    }

    Response handleDeal(const std::string& sessionId) {
        Response res;
        std::string username = getValidatedUsername(sessionId, res);
        if (username.empty()) return res;

        GameTable& table = gm.joinOrCreateTable(username);
        table.join(username);

        res.status_code = 200;
        res.body = table.deal(username);
        addCORSHeaders(res);
        return res;
    }

    Response handleHit(const std::string& sessionId) {
        Response res;
        std::string username = getValidatedUsername(sessionId, res);
        if (username.empty()) return res;

        GameTable* table = gm.getTableForPlayer(username);
        res.status_code = 200;
        res.body = table ? table->hit(username) : "No game found.";
        addCORSHeaders(res);
        return res;
    }

    Response handleStand(const std::string& sessionId) {
        Response res;
        std::string username = getValidatedUsername(sessionId, res);
        if (username.empty()) return res;

        GameTable* table = gm.getTableForPlayer(username);
        res.status_code = 200;
        res.body = table ? table->stand(username) : "No game found.";
        addCORSHeaders(res);
        return res;
    }

    Response handleStatus(const std::string& sessionId) {
        Response res;
        std::string username = getValidatedUsername(sessionId, res);
        if (username.empty()) return res;

        GameTable* table = gm.getTableForPlayer(username);
        res.status_code = 200;
        res.body = table ? table->status() : "No game found.";
        addCORSHeaders(res);
        return res;
    }
}

Response Router::route(const Request& request) {
    std::cout << "[Router] Incoming request: " << request.method << " " << request.path << std::endl;

    const std::string& path = request.path;
    std::string sessionId = extractSessionId(request);

    if (request.method == "OPTIONS") {
        Response res;
        res.status_code = 204;
        addCORSHeaders(res);
        return res;
    }

    if (path == "/login")           return handleLogin(request.body);
    else if (path == "/register")   return handleRegister(request.body);
    else if (path == "/deal")       return handleDeal(sessionId);
    else if (path == "/hit")        return handleHit(sessionId);
    else if (path == "/stand")      return handleStand(sessionId);
    else if (path == "/status")     return handleStatus(sessionId);

    Response res{404, "text/plain", "Not found"};
    addCORSHeaders(res);
    return res;
}