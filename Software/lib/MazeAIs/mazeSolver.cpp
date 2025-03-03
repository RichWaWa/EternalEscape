#include "mazeSolver.h"
#include "screen.h"      // For drawElement and delay
#include "mazeGen.h"     // For isValidCell
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> // for std::shuffle
using namespace std;

/// @brief Recursively attempts to move through the maze one step.
/// @param maze The maze (modified as cells are visited).
/// @param position The current path (a vector of cell coordinates).
/// @return The updated path after making one move (or backtracking if no move is valid).
vector<pair<int, int>> randRecursiveMazeSolver(vector<vector<char>>& maze, vector<pair<int, int>> position, const char playerChar, const char playerPathChar) {
    // Seed the random generator (ideally done once at program start, not per function call)
    //TODO Not needed, we do this at the initialization of the device
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }

    // Shuffle direction indices for random order of movement
    vector<int> directions = {0, 1, 2, 3};
    random_shuffle(directions.begin(), directions.end());

    // Get current player position
    int currentRow = position.back().first;
    int currentCol = position.back().second;

    // Attempt all directions in the randomized order
    for (int i = 0; i < 4; i++) {
        int dir = directions[i];  // Pick a shuffled direction
        
        int newRow = currentRow + pathDirections[dir].first;
        int newCol = currentCol + pathDirections[dir].second;
        int wallRow = currentRow + wallDirections[dir].first;
        int wallCol = currentCol + wallDirections[dir].second;

        // Check if the move is within bounds and the path is open ('0')
        //We may not need the isValidCell. it may break if it gets to the end
        if (isValidCell(newRow, newCol, '0', maze) && maze[wallRow][wallCol] == ',') {  // Check if the wall between is open

            // Mark the current cell with 'x' (except for 'S' and 'E')
            if (maze[currentRow][currentCol] != 'S' && maze[currentRow][currentCol] != 'E') {
                maze[currentRow][currentCol] = playerPathChar;
                drawElement(currentRow, currentCol, playerPathChar);
            }

            // Move forward: Color wall and color player
            drawElement(wallRow, wallCol, playerPathChar);
            position.push_back({newRow, newCol});  // Add new position to path

            // Draw the new player position ('P')
            drawElement(newRow, newCol, playerChar);

            return position;  // Continue moving forward
        }
    }

    // If no moves are possible, backtrack
    if (position.size() > 1) {
        position.pop_back();
        drawElement(currentRow, currentCol, playerPathChar); // Mark backtracked path
    }

    return position;
}
