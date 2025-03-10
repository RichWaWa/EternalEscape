#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <utility>

class Player {
public:
    // Default constructor
    Player();

    // Add a new position to the tracker
    void addPosition(int row, int col);

    // Get a reference to the vector of positions
    std::vector<std::pair<int, int>>& getPositions();

    // Clear all stored positions
    void clearPositions();

private:
    std::vector<std::pair<int, int>> playerPositions;
};

#endif // PLAYERTRACKER_H
