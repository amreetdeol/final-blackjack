#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

class GameTable {
public:
    GameTable(const std::string& id);

    void join(const std::string& username);
    std::string deal(const std::string& username);
    std::string hit(const std::string& username);
    std::string stand(const std::string& username);
    std::string status() const;
    bool isFull() const;

private:
    std::string tableId;
    mutable std::mutex lock;

    std::vector<std::string> players;
    std::unordered_map<std::string, std::vector<int>> hands;
    std::unordered_map<std::string, bool> hasStood;
    std::vector<int> dealerHand;

    int drawCard() const;
    int handValue(const std::vector<int>& hand) const;
    std::string formatHand(const std::vector<int>& hand) const;
};