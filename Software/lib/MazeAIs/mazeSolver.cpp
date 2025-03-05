#include "mazeSolver.h"
#include "screen.h"      // For drawElement and delay
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> // for std::shuffle
using namespace std;

/// @brief Recursively attempts to move through the maze one step.
/// @param maze The maze (modified as cells are visited).
/// @param position The current path (a vector of cell coordinates).
/// @param playerChar The character representing the player.
/// @param playerPathChar The character representing the path the player has taken.
void randRecursiveMazeSolver(vector<vector<char>>& maze, vector<pair<int, int>>& position, const char playerChar, const char playerPathChar) {
    // Ensure there is a valid starting position
    if (position.empty()) {
        printDebugInfo("Error: Player position vector is empty!");
        return;
    }


    // Shuffle direction indices for random order of movement
    vector<int> directions = {0, 1, 2, 3};
    random_shuffle(directions.begin(), directions.end());
    Serial.print("Shuffled directions: ");
    for (int dir : directions) {
        Serial.print(dir);
        Serial.print(" ");
    }
    Serial.println(); // Move to a new line after printing all elements

    // Get current player position
    int currentRow = position.back().first;
    int currentCol = position.back().second;

    printDebugInfo("Current Position", currentRow, currentCol);

    // Attempt all directions in the randomized order
    for (int i = 0; i < 4; i++) {
        int dir = directions[i];  // Pick a shuffled direction
        Serial.println(dir);
        int newRow = currentRow + pathDirections[dir].first;
        int newCol = currentCol + pathDirections[dir].second;
        printDebugInfo("Attempting to move to", newRow, newCol);
        int wallRow = currentRow + wallDirections[dir].first;
        int wallCol = currentCol + wallDirections[dir].second;
        printDebugInfo("Attempting to verify wall at", wallRow, wallCol);
        printDebugInfo("Directions Computed");
        // Validate move
        if (!isValidMove(newRow, newCol, wallRow, wallCol, maze)) {
            printDebugInfo("Attempted move was not valid");
            continue;
        }

        printDebugInfo("Moving to", newRow, newCol);

        // Mark current position with trail if not start or end
        if (maze[currentRow][currentCol] != 'S' && maze[currentRow][currentCol] != 'E') {
            maze[currentRow][currentCol] = playerPathChar;
            drawElement(currentRow, currentCol, playerPathChar);
            printDebugInfo("Marked trail at current position");
        }

        // Update the wall and new position
        drawElement(wallRow, wallCol, playerPathChar);
        position.push_back({newRow, newCol});

        // Draw new player position
        drawElement(newRow, newCol, playerChar);
        return;
    }

    // No moves possible, backtracking
    if (position.size() > 1) {
        printDebugInfo("No valid moves, backtracking...");
        position.pop_back();
        drawElement(currentRow, currentCol, playerPathChar);
    } else {
        printDebugInfo("Player is stuck with no way to backtrack.");
    }
}

/// @brief Prints debug information for the current solver state.
/// @param message Debug message.
/// @param row Row index (optional).
/// @param col Column index (optional).
void printDebugInfo(const char* message, int row, int col) {
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

/// @brief Checks if a move is valid.
/// @param newRow New row position.
/// @param newCol New column position.
/// @param wallRow Row of the wall being crossed.
/// @param wallCol Column of the wall being crossed.
/// @param maze The maze.
/// @return True if the move is valid.
bool isValidMove(int newRow, int newCol, int wallRow, int wallCol, const vector<vector<char>>& maze) {
    // Check if the new position is within bounds and a valid path
    Serial.println("Validating Move");
    // First, check if the row and column are within valid bounds.
    if ((newRow <= 0 || newRow >= maze.size() - 1 || newCol <= 0 || newCol >= maze[0].size() - 1) || !(maze[newRow][newCol] == '0')) {
        printDebugInfo("Out of bounds or invalid path", newRow, newCol);
        return false;
    }
    Serial.println("Checked Path");
    // Check if the wall is a valid passage
    if (wallRow <= 0 || wallRow >= maze.size() - 1 || wallCol <= 0 || wallCol >= maze[0].size() - 1) {
        printDebugInfo("Wall out of bounds", wallRow, wallCol);
        return false;
    }
    Serial.println("Checked Wall Bounds");
    if (maze[wallRow][wallCol] != ',') {
        printDebugInfo("Wall is not a valid passage", wallRow, wallCol);
        return false;
    }
    Serial.println("Checked Wall passable");
    return true;
}
