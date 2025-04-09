#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <semaphore>
#include <random>

class GameSession {
public:
    explicit GameSession(const std::string& username);
    std::string deal();
    std::string hit();
    std::string stand();
    std::string status();

private:
    std::string player;
    std::vector<int> player_hand;
    std::vector<int> dealer_hand;

    std::mutex stateLock;
    std::binary_semaphore turnLock{1};

    int drawCard();
    int handValue(const std::vector<int>& hand);
};