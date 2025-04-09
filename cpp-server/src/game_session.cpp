#include "game_session.hpp"
#include <sstream>
#include <random>

GameSession::GameSession(const std::string& username) : player(username) {}

int GameSession::drawCard() {
    return rand() % 11 + 1;  // card between 1–11
}

int GameSession::handValue(const std::vector<int>& hand) {
    int sum = 0;
    for (int c : hand) sum += c;
    return sum;
}

std::string GameSession::deal() {
    std::lock_guard<std::mutex> guard(stateLock);
    player_hand = { drawCard(), drawCard() };
    dealer_hand = { drawCard(), drawCard() };
    return status();
}

std::string GameSession::hit() {
    std::lock_guard<std::mutex> guard(stateLock);
    player_hand.push_back(drawCard());
    return status();
}

std::string GameSession::stand() {
    std::lock_guard<std::mutex> guard(stateLock);
    while (handValue(dealer_hand) < 17) {
        dealer_hand.push_back(drawCard());
    }
    return status();
}

std::string GameSession::status() {
    std::ostringstream ss;
    ss << "Your hand: ";
    for (int c : player_hand) ss << c << " ";
    ss << " (" << handValue(player_hand) << ")\n";
    ss << "Dealer hand: ";
    for (int c : dealer_hand) ss << c << " ";
    ss << " (" << handValue(dealer_hand) << ")";
    return ss.str();
}