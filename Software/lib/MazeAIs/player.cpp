#include "player.h"

// Constructor
Player::Player() {
    // Initialize if needed
}

// Add a new position
void Player::addPosition(int row, int col) {
    playerPositions.push_back({row, col});
}

// Get the positions vector
const std::vector<std::pair<int, int>>& Player::getPositions() const {
    return playerPositions;
}

// Clear all positions
void Player::clearPositions() {
    playerPositions.clear();
}
