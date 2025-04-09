#include "game_manager.hpp"
#include <sstream>

GameManager& GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

GameTable& GameManager::joinOrCreateTable(const std::string& username) {
    std::lock_guard<std::mutex> guard(lock);

    // Already joined a table
    if (playerToTable.find(username) != playerToTable.end()) {
        const std::string& tableId = playerToTable[username];
        return *tables[tableId];
    }

    // Reuse table if not full
    std::shared_ptr<GameTable> table;
    std::string tableId;

    if (!tables.empty()) {
        for (auto& [id, tbl] : tables) {
            if (!tbl->isFull()) {
                table = tbl;
                tableId = id;
                break;
            }
        }
    }

    // If no table found, create new one
    if (!table) {
        std::ostringstream oss;
        oss << "table-" << nextTableId++;
        tableId = oss.str();
        table = std::make_shared<GameTable>(tableId);
        tables[tableId] = table;
    }

    // Record assignment
    playerToTable[username] = tableId;
    table->join(username);  // <-- 🟢 Important fix here

    return *table;
}

GameTable* GameManager::getTableForPlayer(const std::string& username) {
    std::lock_guard<std::mutex> guard(lock);
    if (playerToTable.count(username)) {
        const std::string& tableId = playerToTable[username];
        if (tables.count(tableId)) {
            return tables[tableId].get();
        }
    }
    return nullptr;
}