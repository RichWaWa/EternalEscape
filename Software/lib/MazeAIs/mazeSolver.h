#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <vector>
#include <utility>
using namespace std;
//Player class
class Player {
public:
    // Default constructor: OR = Orange, BL = Blue
    Player(const char& color = 'O');

    // Add a new position to the tracker
    void addPosition(int row, int col);

    // Get a reference to the vector of positions
    vector<pair<int, int>>& getPositions();

    //calculate the player's move
    void calculateMove(vector<vector<char>>& maze);
    
    // Clear all stored positions
    void clearPositions();

private:
    //stores player positions
    vector<pair<int, int>> playerPositions;
    //stores the direction choices for those positions. The lists should line
    vector<vector<int>> directionsList;
    //const list of where the paths are at{S,N,W,E}
    const vector<pair<int, int>> pathDirections = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
    //const list of where the walls are at
    const vector<pair<int, int>> wallDirections = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    //shuffles the movement directions
    vector<int> shuffleDirections();
    //Checks if a move is valid to do.
    bool isValidMove(int newRow, int newCol, int wallRow, int wallCol, const vector<vector<char>>& maze);
    //Function for printing debug info. Basically formats the coords better
    void printDebugInfo(const char* message, int row = -1, int col = -1);
};
#endif

