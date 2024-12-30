#include "mazeGen.h"
#include "screen.h"
#include <vector>
#include <cstdlib>
#include <ctime>
/*
Implementation of Prims algorithm guided by the following sources
https://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm
https://github.com/jamis/csmazes
*/
char maze[31][41]; //the maze which will be built
//Function 
void generateMaze() {
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
            // Mark the starting point with 'S'
            maze[start.row][start.col] = 'S';
            drawElement(start.row, start.col, 'S');
            foundStart = true;
        }
    }
    
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
    }
}

//Call this to draw the maze elements
void drawElement(int rowA, int colA, char cellType) {
    // Maze cell dimensions
    const int cellSize = 14;
    const int wallThickness = 2;
    const int offset = 1; // Account for the 1-pixel border around the maze area

    // Colors in 16-bit RGB565 format
    const uint16_t BLACK = 0x0000; 
    const uint16_t WHITE = 0xFFFF; 
    const uint16_t GREEN = 0x07E0; 
    const uint16_t RED = 0xF800; 
    const uint16_t GREY = 0xC618; 
    const uint16_t DARKGREY = 0x7BEF;

    // Calculate the top-left pixel coordinates of the cell
    int x = offset + colA * cellSize;
    int y = offset + rowA * cellSize;

    // Determine what to draw based on cellType
    switch (cellType) {
        case '.': // Path cell (black 14x14)
            drawFillRectangle(x, y, cellSize, cellSize, BLACK);
            break;

        case '#': // Wall cell
            if (colA % 2 == 1 && rowA % 2 == 0) {
                // Odd column, even row: Horizontal wall
                drawFillRectangle(x, y + (cellSize / 2 - 1), cellSize, wallThickness, WHITE);
            } else if (colA % 2 == 0 && rowA % 2 == 1) {
                // Even column, odd row: Vertical wall
                drawFillRectangle(x + (cellSize / 2 - 1), y, wallThickness, cellSize, WHITE);
            } else {
                // Default fallback for unexpected wall cases
                drawFillRectangle(x, y, cellSize, cellSize, GREEN);
            }
            break;

        case 'C': // Corner cell (white 2x2)
            drawFillRectangle(x + (cellSize / 2 - 1), y + (cellSize / 2 - 1), wallThickness, wallThickness, WHITE);
            break;

        case 'E': // End
            drawFinishSquare(x, y, cellSize); // Draws Checkered Shape
            break;

        case 'S': // Start
            drawFillRectangle(x, y, cellSize, cellSize, GREY); // Red
            break;

        default: // Default case (green 14x14)
            drawFillRectangle(x, y, cellSize, cellSize, GREEN);
            break;
    }
}
