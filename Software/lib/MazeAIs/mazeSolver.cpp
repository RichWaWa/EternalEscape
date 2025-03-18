#include "mazeSolver.h"
#include "screen.h"      // For drawElement and delay
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> // for std::shuffle
using namespace std;

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
    directionsList.clear();
}

/// @brief Recursively attempts to move through the maze one step.
/// @param maze The maze (modified as cells are visited).
/// @param position The current path (a vector of cell coordinates).
/// @param playerChar The character representing the player.
/// @param playerPathChar The character representing the path the player has taken.
void Player::calculateMove(vector<vector<char>>& maze, const char playerChar, const char playerPathChar) {
    // Ensure there is a valid starting position
    if (playerPositions.empty()) {
        printDebugInfo("Error: Player position vector is empty!");
        return;
    }
    //Check if NOT first run
    if(!directionsList.empty()){
        //Need to somehow determine if we are backtracking here
        //See if the length of the paths list is 1 shorter than the Directions list. If it is, then we likely backtracked.
        //If this is true, then we know we have backtracked.
        if(directionsList.size() > playerPositions.size()){
            Serial.println("Vector lists unequal. We must of backtracked.");
            directionsList.pop_back(); 
        }else{
            //New move, shuffle and pop back.
            directionsList.push_back(shuffleDirections());
        }
    }else{
        //Firstrun condition
        Serial.println("First Run Detected");
        directionsList.push_back(shuffleDirections());
    }
    // Get current player position
    int currentRow = playerPositions.back().first;
    int currentCol = playerPositions.back().second;

    printDebugInfo("Current Position", currentRow, currentCol);

    // Attempt all directions in the randomized order
    for (int i = 0; i < 4; i++) {
        int dir = directionsList.back()[i];  // Pick a shuffled direction from the end of the list
        Serial.println("Moving in direction" + dir);
        int newRow = currentRow + pathDirections[dir].first;
        int newCol = currentCol + pathDirections[dir].second;
        printDebugInfo("I want to move to", newRow, newCol);
        int wallRow = currentRow + wallDirections[dir].first;
        int wallCol = currentCol + wallDirections[dir].second;
        printDebugInfo("I want to verify wall at", wallRow, wallCol);
        // Validate move
        if (!isValidMove(newRow, newCol, wallRow, wallCol, maze)) {
            printDebugInfo("Attempted move was not valid");
            continue;
        }
        //idea of this is to rotate the directions vector so that the first position is a direction we havnt tried yet.
        rotate(directionsList.back().begin(), directionsList.back().begin() + i, directionsList.back().end());
        printDebugInfo("Moving to", newRow, newCol);

        // Mark current position with trail if not start or end
        if (maze[currentRow][currentCol] != 'S' && maze[currentRow][currentCol] != 'E') {
            maze[currentRow][currentCol] = playerPathChar;
            drawElement(currentRow, currentCol, playerPathChar);
            printDebugInfo("Marked trail at current position");
        }

        // Update the wall and new position
        drawElement(wallRow, wallCol, playerPathChar);
        playerPositions.push_back({newRow, newCol});

        // Draw new player position
        drawElement(newRow, newCol, playerChar);
        return;
    }

    // No moves possible, backtracking
    if (playerPositions.size() > 1) {
        printDebugInfo("No valid moves, backtracking...");
        playerPositions.pop_back();
        //NOTE: We dont want to pop_back the directions vector, because we use it to determine if we backtracked or not
        drawElement(currentRow, currentCol, playerPathChar);
    } else {
        printDebugInfo("Player is stuck with no way to backtrack.");
    }
}

/// @brief shuffles the movement directions to make player movement more unique
vector<int> Player::shuffleDirections(){
    // Shuffle direction indices for random order of movement
    vector<int> directions = {0, 1, 2, 3};
    random_shuffle(directions.begin(), directions.end());
    Serial.print("Shuffled directions: ");
    for (int dir : directions) {
        Serial.print(dir);
        Serial.print(" ");
    }
    Serial.println(); // Move to a new line after printing all elements
    return directions;
}

/// @brief Checks if a move is valid.
/// @param newRow New row position.
/// @param newCol New column position.
/// @param wallRow Row of the wall being crossed.
/// @param wallCol Column of the wall being crossed.
/// @param maze The maze.
/// @return True if the move is valid.
bool Player::isValidMove(int newRow, int newCol, int wallRow, int wallCol, const vector<vector<char>>& mazeCheck) {
    // Check if the new position is within bounds and a valid path
    Serial.println("Validating Move");
    // First, check if the row and column are within valid bounds.
    if (newRow <= 0 || newRow >= mazeCheck.size() - 1 || newCol <= 0 || newCol >= mazeCheck[0].size() - 1) {
        printDebugInfo("Out of bounds", newRow, newCol);
        return false;
    }
    
    // Now it's safe to access maze[newRow][newCol]. Check if it's a valid path.
    if (mazeCheck[newRow][newCol] != '0') {
        printDebugInfo("Invalid path cell", newRow, newCol);
        return false;
    }

    Serial.println("Checked Path");
    // Check if the wall is a valid passage
    if (wallRow <= 0 || wallRow >= mazeCheck.size() - 1 || wallCol <= 0 || wallCol >= mazeCheck[0].size() - 1) {
        printDebugInfo("Wall out of bounds", wallRow, wallCol);
        return false;
    }
    Serial.println("Checked Wall Bounds");
    if (mazeCheck[wallRow][wallCol] != ',') {
        printDebugInfo("Wall is not a valid passage", wallRow, wallCol);
        return false;
    }
    Serial.println("Checked Wall passable");
    return true;
}

/// @brief Prints debug information for the current solver state.
/// @param message Debug message.
/// @param row Row index (optional).
/// @param col Column index (optional).
void Player::printDebugInfo(const char* message, int row, int col) {
    Serial.print(message);
    if (row != -1 && col != -1) { // Only print coordinates if provided
        Serial.print(" (");
        Serial.print(row);
        Serial.print(", ");
        Serial.print(col);
        Serial.print(")");
    }
    Serial.println();
}
