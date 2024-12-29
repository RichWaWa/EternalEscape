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
    // Copy the maze template
    for (int row = 0; row < 31; ++row) {
        for (int col = 0; col < 41; ++col) {
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
        return row > 0 && row < 30 && col > 0 && col < 40 &&
               maze[row][col] == '#' && maze[row][col] != 'C';
    };

    // Find the starting position (a '.' on the maze template)
    Cell start;
    bool foundStart = false;
    for (int row = 1; row < 30 && !foundStart; ++row) {
        for (int col = 1; col < 40 && !foundStart; ++col) {
            if (maze[row][col] == '.') {
                start = {row, col};
                foundStart = true;
            }
        }
    }

    // Mark the starting point with 'S'
    maze[start.row][start.col] = 'S';

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

            if (oppositeRow > 0 && oppositeRow < 30 &&
                oppositeCol > 0 && oppositeCol < 40 &&
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

    // Place an exit ('E') on a border wall
    bool exitPlaced = false;
    for (int col = 1; col < 40 && !exitPlaced; ++col) {
        if (maze[30][col] == '*') {
            maze[30][col] = 'E';
            exitPlaced = true;
        }
    }
}
