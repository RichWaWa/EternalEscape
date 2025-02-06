#include "mazeGen.h"
#include "screen.h"
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
/*
Implementation of Prims algorithm guided by the following sources
https://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm
https://github.com/jamis/csmazes
*/
//char maze[31][41]; //the maze which will be built
vector<vector<char>>  maze;

// Directions for movement (up, down, left, right)
const vector<pair<int, int>> directions = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

// Function to check if a cell is a valid candidate for maze expansion
bool isValidCell(int row, int col, const vector<vector<char>>& maze) {
    // Ensure row and column indices are within bounds (avoiding edges)
    bool withinBounds = (row > 0 && row < maze.size() - 1) && (col > 0 && col < maze[0].size() - 1);

    // Ensure the cell is a wall ('#') and can be carved into a path
    bool isWall = (maze[row][col] == '#');

    return withinBounds && isWall;
}

// Prim's Algorithm to generate the maze
void generateMaze() {
    //Copy the template
    vector<vector<char>> maze = copyMazeTemplate();

    //Place start ('S') and end ('E')
    placeStartAndEnd(maze);

    //Render the maze to the display
    renderMaze(maze);

    srand(time(0));  // Seed random number generator

    // Find start ('S') and end ('E') positions
    //TODO change this so the positions are saved.
    int startRow = 0, startCol = 0;
    int endRow = 0, endCol = 0;
    for (int row = 0; row < maze.size(); row++) {
        for (int col = 0; col < maze[row].size(); col++) {
            if (maze[row][col] == 'S') {
                startRow = row;
                startCol = col;
            }
            if (maze[row][col] == 'E') {
                endRow = row;
                endCol = col;
            }
        }
    }

    // If start or end not found, exit early
    if (startRow == -1 || startCol == -1 || endRow == -1 || endCol == -1) return;

    // Initialize frontier list with walls adjacent to start
    vector<pair<int, int>> frontier;
    maze[startRow][startCol] = '.'; // Mark start as a path

    for (auto [dr, dc] : directions) {
        int newRow = startRow + dr;
        int newCol = startCol + dc;
        if (isValidCell(newRow, newCol, maze)) {
            frontier.push_back({newRow, newCol});
            maze[newRow][newCol] = ','; // Mark as frontier
            drawElement(newRow, newCol, 'F'); // LIGHTRED
        }
    }

    // Process frontier list
    while (!frontier.empty()) {
        // Pick a random frontier cell
        int randIndex = rand() % frontier.size();
        auto [cellRow, cellCol] = frontier[randIndex];
        frontier.erase(frontier.begin() + randIndex);

        // Check neighbors to connect to the maze
        vector<pair<int, int>> neighbors;
        for (auto [dr, dc] : directions) {
            int newRow = cellRow + dr;
            int newCol = cellCol + dc;
            if (newRow > 0 && newRow < maze.size() - 1 &&
                newCol > 0 && newCol < maze[0].size() - 1 &&
                maze[newRow][newCol] == '.') {
                neighbors.push_back({newRow, newCol});
            }
        }

        if (!neighbors.empty()) {
            // Choose a random connected path cell
            auto [neighborRow, neighborCol] = neighbors[rand() % neighbors.size()];

            // Remove the wall between them
            int wallRow = (cellRow + neighborRow) / 2;
            int wallCol = (cellCol + neighborCol) / 2;
            maze[wallRow][wallCol] = '.';
            drawElement(wallRow, wallCol, '.'); // Black

            // Mark new cell as part of the maze
            maze[cellRow][cellCol] = '.';
            drawElement(cellRow, cellCol, '.'); // Black

            // Add new frontier cells
            for (auto [dr, dc] : directions) {
                int newRow = cellRow + dr;
                int newCol = cellCol + dc;
                if (isValidCell(newRow, newCol, maze)) {
                    frontier.push_back({newRow, newCol});
                    maze[newRow][newCol] = ','; // Mark as frontier
                    drawElement(newRow, newCol, 'F'); // LIGHTRED
                }
            }
        }
    }

    // Ensure 'E' remains unchanged
    drawElement(endRow, endCol, 'E'); // Checkered finish
}

// Function to copy the template
vector<vector<char>> copyMazeTemplate() {
    return mazeTemplate;
}

// Function to choose a random "#" for start ('S') and end ('E')
void placeStartAndEnd(vector<vector<char>>& maze) {
    int rows = maze.size();
    int cols = maze[0].size();

    srand(time(0)); // Seed for random number generation

    // Randomly select start
    bool startPlaced = false;
    while (!startPlaced) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (maze[row][col] == '.') {
            maze[row][col] = 'S';
            startPlaced = true;
        }
    }

    // Randomly select end
    bool endPlaced = false;
    while (!endPlaced) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (maze[row][col] == '.' && maze[row][col] != 'S') {
            maze[row][col] = 'E';
            endPlaced = true;
        }
    }
}

// Function to render the maze to the display
void renderMaze(const vector<vector<char>>& maze) {
    drawFillScreen(DARKGREY); //fill orange
    //start at row one to exclude the border
    for (int row = 1; row < maze.size() - 1; ++row) {
        for (int col = 1; col < maze[row].size() - 1; ++col) {
            drawElement(row, col, maze[row][col]);
        }
    }
}


