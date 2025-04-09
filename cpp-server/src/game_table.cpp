#include "game_table.hpp"
#include <random>
#include <sstream>
#include <algorithm>

GameTable::GameTable(const std::string& id) : tableId(id) {}

void GameTable::join(const std::string& username) {
    std::lock_guard<std::mutex> guard(lock);
    if (std::find(players.begin(), players.end(), username) == players.end()) {
        players.push_back(username);
    }
}

bool GameTable::isFull() const {
    std::lock_guard<std::mutex> guard(lock);
    return players.size() >= 4;
}

int GameTable::drawCard() const {
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 11); // basic for now
    return dist(rng);
}

int GameTable::handValue(const std::vector<int>& hand) const {
    int sum = 0;
    for (int card : hand) sum += card;
    return sum;
}

std::string GameTable::formatHand(const std::vector<int>& hand) const {
    std::ostringstream oss;
    for (int c : hand) oss << c << " ";
    oss << " (" << handValue(hand) << ")";
    return oss.str();
}

std::string GameTable::deal(const std::string& username) {
    std::lock_guard<std::mutex> guard(lock);

    if (hands[username].empty()) {
        hands[username].push_back(drawCard());
        hands[username].push_back(drawCard());
    }

    if (dealerHand.empty()) {
        dealerHand.push_back(drawCard());
        dealerHand.push_back(drawCard());
    }

    std::ostringstream oss;
    oss << "Your hand: " << formatHand(hands[username]) << "\n";
    oss << "Dealer hand: " << dealerHand[0] << " ? (hidden)";
    return oss.str();
}

std::string GameTable::hit(const std::string& username) {
    std::lock_guard<std::mutex> guard(lock);

    if (hands[username].empty()) return "You must deal first.";
    hands[username].push_back(drawCard());

    int val = handValue(hands[username]);
    if (val > 21) return "Busted! Your hand: " + formatHand(hands[username]);
    return "Hit successful. Your hand: " + formatHand(hands[username]);
}

std::string GameTable::stand(const std::string& username) {
    std::lock_guard<std::mutex> guard(lock);

    hasStood[username] = true;

    std::ostringstream oss;
    oss << username << " stands.\n";

    // Check if all players have stood
    bool allStood = true;
    for (const auto& p : players) {
        if (!hasStood[p]) {
            allStood = false;
            break;
        }
    }

    // If everyone stood, dealer plays
    if (allStood) {
        while (handValue(dealerHand) < 17) {
            dealerHand.push_back(drawCard());
        }

        int dealerScore = handValue(dealerHand);
        oss << "\nDealer hand: " << formatHand(dealerHand) << "\n";

        // Determine results for each player
        for (const auto& p : players) {
            int playerScore = handValue(hands[p]);
            oss << p << ": " << formatHand(hands[p]);

            if (playerScore > 21) {
                oss << " - BUSTED\n";
            } else if (dealerScore > 21 || playerScore > dealerScore) {
                oss << " - WIN\n";
            } else if (playerScore == dealerScore) {
                oss << " - PUSH\n";
            } else {
                oss << " - LOSE\n";
            }
        }
    } else {
        oss << "Waiting for others to stand...\n";
    }

    return oss.str();
}

std::string GameTable::status() const {
    std::lock_guard<std::mutex> guard(lock);
    std::ostringstream oss;
    oss << "Table ID: " << tableId << "\n";
    for (const auto& p : players) {
        oss << p << ": " << formatHand(hands.at(p)) << "\n";
    }
    return oss.str();
}