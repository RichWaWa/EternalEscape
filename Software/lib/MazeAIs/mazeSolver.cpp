#include "mazeSolver.h"
#include "screen.h"      // For drawElement and delay
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> // for std::shuffle
using namespace std;

//working vars

// Constructor
Player::Player(const char& color) {
    if (color == 'B') {
        playerChar = 'B';
        playerPathChar = 'Z';       //cap Z
        playerBacktrackChar = 'z';  //lowercase z
    } else {
        playerChar = 'O';
        playerPathChar = 'X';       //cap X
        playerBacktrackChar = 'x';  //lowercase x
    }
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
/// @brief This algorithm relies on the princible that in prims algorithm, there is exactly ONE way to get to any part of the maze, given any4 single starting point.
/// @param maze The maze (modified as cells are visited).
void Player::calculateMove(vector<vector<char>>& maze) {
    //Serial.println("=========================================");
    // Ensure there is a valid starting position
    if (playerPositions.empty()) {
        printDebugInfo("Error: Player position vector is empty!");
        return;
    }
    //Check if NOT first run
    if(!directionsList.empty()){
        //See if the length of the paths list is 1 shorter than the Directions list. If it is, then we likely backtracked.
        //If this is true, then we know we have backtracked.
        if(directionsList.size() > playerPositions.size()){
            //Serial.println("Vector lists unequal. We must of backtracked.");
            directionsList.pop_back(); 
        }else{
            //New move, shuffle and push back.
            directionsList.push_back(shuffleDirections());
        }
    }else{
        //Firstrun condition
        //Serial.println("First Run Detected");
        directionsList.push_back(shuffleDirections());
    }
    // Get current player position
    int currentRow = playerPositions.back().first;
    int currentCol = playerPositions.back().second;
    //printDebugInfo("Current Position", currentRow, currentCol);

    // Attempt all directions in the randomized order. Dont change the length to 5!!! Leave at 4 to avoid infinite looping.
    for (int i = 0; i < 4; i++) {
        int dir = directionsList.back()[i];  // Pick a shuffled direction from the end of the list
        //If this is the character 9, we know we have looped all the way through, so we need to break out of this loop.
        if(dir == 9){
            //if we reached the end of the direction list, break out
            break;
        }
        //Serial.print("Rotated Directions: ");
        //for (int N : directionsList.back()) {
        //    Serial.print(N);
        //    Serial.print(" ");
        //}
        //Serial.println(); // Move to a new line after printing all elements
        //Serial.println("Moving in direction" + dir);
        int newRow = currentRow + pathDirections[dir].first;
        int newCol = currentCol + pathDirections[dir].second;
        //printDebugInfo("I want to move to", newRow, newCol);
        int wallRow = currentRow + wallDirections[dir].first;
        int wallCol = currentCol + wallDirections[dir].second;
        //printDebugInfo("I want to verify wall at", wallRow, wallCol);
        // Validate move
        if (!isValidMove(newRow, newCol, wallRow, wallCol, maze)) {
            //printDebugInfo("Attempted move was not valid");
            continue;
        }
        //idea of this is to rotate the directions vector so that the first position is a direction we havnt tried yet.
        rotate(directionsList.back().begin(), directionsList.back().begin() + (i+1), directionsList.back().end());
        //Serial.printf("Rotating: %d\n", (i + 1));
        //Serial.print("Rotated Directions: ");
        //for (int N : directionsList.back()) {
        //    Serial.print(N);
        //    Serial.print(" ");
        //}
        //Serial.println(); // Move to a new line after printing all elements
        //printDebugInfo("Moving to", newRow, newCol);

        //draw path
        drawElement(currentRow, currentCol, playerPathChar);
        // Update the wall and new position
        drawElement(wallRow, wallCol, playerPathChar);
        playerPositions.push_back({newRow, newCol});
        // Draw new player position
        drawElement(newRow, newCol, playerChar);
        return;
    }

// No moves possible, backtracking
    if (playerPositions.size() > 1) {
    //printDebugInfo("No valid moves, backtracking...");
    int currentRow = playerPositions.back().first;
    int currentCol = playerPositions.back().second;

    playerPositions.pop_back();

    int backtrackRow = playerPositions.back().first;
    int backtrackCol = playerPositions.back().second;

    // Draw backtrack on the current position
    drawElement(currentRow, currentCol, playerBacktrackChar);

    // Calculate the wall position between current and backtrack positions
    int wallRow = (currentRow + backtrackRow) / 2;
    int wallCol = (currentCol + backtrackCol) / 2;

    // Color the wall with the backtrack color
    drawElement(wallRow, wallCol, playerBacktrackChar);

    // Color the backtracked-to position with the player color
    drawElement(backtrackRow, backtrackCol, playerChar);
    } else {
        printDebugInfo("Player is stuck with no way to backtrack.");
    }
}

/// @brief shuffles the movement directions to make player movement more unique
vector<int> Player::shuffleDirections(){
    // Shuffle direction indices for random order of movement
    vector<int> directions = {0, 1, 2, 3};
    random_shuffle(directions.begin(), directions.end());
    //Serial.print("Shuffled directions: ");
    //for (int dir : directions) {
    //    Serial.print(dir);
    //    Serial.print(" ");
    //}
    //Serial.println(); // Move to a new line after printing all elements

    //We add 9 at the end of the list to signify when we have tried every direction.
    //This prevents it from getting stuck. Ex output {0, 1, 2, 3, 9}; 
    directions.push_back(9);    
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
    //Serial.println("Validating Move");
    // First, check if the row and column are within valid bounds.
    if (newRow <= 0 || newRow >= mazeCheck.size() - 1 || newCol <= 0 || newCol >= mazeCheck[0].size() - 1) {
        return false;
    }
    // Now it's safe to access maze[newRow][newCol].
    //Check if the path is valid or is the end.
    if (mazeCheck[newRow][newCol] != '0' && mazeCheck[newRow][newCol] != 'E') {
        return false;
    }
    // Check if the wall is a valid passage
    if (wallRow <= 0 || wallRow >= mazeCheck.size() - 1 || wallCol <= 0 || wallCol >= mazeCheck[0].size() - 1) {
        return false;
    }
    if (mazeCheck[wallRow][wallCol] != ',') {
        return false;
    }
    // Check if the new position is the previous position to prevent reversing
    if (playerPositions.size() > 1) {
        std::pair<int, int> prevPosition = playerPositions[playerPositions.size() - 2];
        if (newRow == prevPosition.first && newCol == prevPosition.second) {
            return false;
        }
    }
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
