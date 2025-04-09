#pragma once
#include "game_table.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>

class GameManager {
public:
    static GameManager& getInstance();

    GameTable& joinOrCreateTable(const std::string& username);
    GameTable* getTableForPlayer(const std::string& username);

private:
    GameManager() = default;

    std::mutex lock;
    std::unordered_map<std::string, std::shared_ptr<GameTable>> tables;
    std::unordered_map<std::string, std::string> playerToTable;

    int nextTableId = 1;
};