#include "player.h"

// Constructor
Player::Player() {
    // Initialize if needed
}

// Add a new position
void Player::addPosition(int row, int col) {
    playerPositions.push_back({row, col});
}

// Get the positions vector pair
std::vector<std::pair<int, int>>& Player::getPositions(){
    return playerPositions;
}

// Clear all positions
void Player::clearPositions() {
    playerPositions.clear();
}
