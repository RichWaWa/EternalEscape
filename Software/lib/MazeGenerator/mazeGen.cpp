#include "mazeGen.h"
#include <vector>
#include <cstdlib>
#include <ctime>
/*
Implementation of Prims algorithm guided by the following sources
https://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm
https://github.com/jamis/csmazes
*/


void generateMaze(char maze[31][41]) {
    const int mazeHeight = 31;
    const int mazeWidth = 41;
    // Copy the maze template
    for (int row = 0; row < mazeHeight; ++row) {
        for (int col = 0; col < mazeWidth; ++col) {
            maze[row][col] = mazeTemplate[row][col];
        }
    }

    // Seed random number generator
    srand(static_cast<unsigned>(time(0)));

    // Structure for representing a position in the maze
    struct Cell {
        int row, col;
    };

    // Directions for neighbors: {row_offset, col_offset}
    const Cell directions[] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
    
    // List of walls to consider for carving
    std::vector<Cell> walls;

    // Helper function to check if a position is valid
    auto isValid = [&](int row, int col) {
        return row > 0 && row < mazeHeight && col > 0 && col < mazeWidth &&
               maze[row][col] == '#' && maze[row][col] != 'C';
    };

    // Find the starting position (a '.' on the maze template)
    // Find a random starting position (a '.' on the maze template)
    Cell start;
    bool foundStart = false;
    while (!foundStart) {
        // Randomly choose a row and column within the bounds
        int row = 1 + rand() % (mazeHeight - 2); // Rows between 1 and 30
        int col = 1 + rand() % (mazeWidth - 2); // Columns between 1 and 40

        // Check if the selected cell is a valid starting point
        if (maze[row][col] == '.') {
            start = {row, col};
            foundStart = true;
        }
    }
    // Mark the starting point with 'S'
    maze[start.row][start.col] = 'S';

    // Place an exit ('E') on a border wall
    //place exit(Gotta make sure the algo can work around this too)
    bool exitPlaced = false;

    while (!exitPlaced) {
        // Randomly select one of the edge regions
        int edge = rand() % 4; // 0: first row, 1: last row, 2: first column, 3: last column
        int row, col;

        switch (edge) {
            case 0: // First row (row 1, columns 1-40)
                row = 1;
                col = 1 + rand() % (mazeWidth - 2); // Columns 1 to 40
                break;

            case 1: // Last row (row 30, columns 1-40)
                row = 30;
                col = 1 + rand() % (mazeWidth - 2); // Columns 1 to 40
                break;

            case 2: // First column (rows 1-30, column 1)
                row = 1 + rand() % (mazeHeight - 2); // Rows 1 to 29
                col = 1;
                break;

            case 3: // Last column (rows 1-30, column 40)
                row = 1 + rand() % (mazeHeight - 2); // Rows 1 to 30
                col = 40;
                break;
        }

        // Check if the selected position is a valid path
        if (maze[row][col] == '.') {
            maze[row][col] = 'E'; // Place the exit
            exitPlaced = true;
        }
    }

    // Add neighboring walls of the start cell to the walls list
    for (const auto& dir : directions) {
        int newRow = start.row + dir.row;
        int newCol = start.col + dir.col;
        if (isValid(newRow, newCol)) {
            walls.push_back({newRow, newCol});
        }
    }

    // Main loop for Prim's Algorithm
    while (!walls.empty()) {
        // Pick a random wall
        int randomIndex = rand() % walls.size();
        Cell wall = walls[randomIndex];
        walls.erase(walls.begin() + randomIndex);

        // Find the opposite cell
        for (const auto& dir : directions) {
            int oppositeRow = wall.row + dir.row;
            int oppositeCol = wall.col + dir.col;

            if (oppositeRow > 0 && oppositeRow < (mazeHeight - 2) &&
                oppositeCol > 0 && oppositeCol < (mazeWidth - 2) &&
                maze[oppositeRow][oppositeCol] == '.') {
                
                // Carve path through the wall and mark it as path
                maze[wall.row][wall.col] = '.';
                int newPathRow = (wall.row + oppositeRow) / 2;
                int newPathCol = (wall.col + oppositeCol) / 2;
                maze[newPathRow][newPathCol] = '.';

                // Add new walls to the list
                for (const auto& newDir : directions) {
                    int newWallRow = wall.row + newDir.row;
                    int newWallCol = wall.col + newDir.col;
                    if (isValid(newWallRow, newWallCol)) {
                        walls.push_back({newWallRow, newWallCol});
                    }
                }
                break;
            }
        }
    }
}