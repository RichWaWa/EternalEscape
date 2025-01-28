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
//Function 
void generateMaze() {
    //First, copy the template maze "mazeTemplate"
    //next, choose a random "#" too be the start, and another different "# to be the finish"
    //Lastly, print the template maze to the display using the drawElement function.

    // Step 1: Copy the template
    vector<vector<char>> maze = copyMazeTemplate();

    // Step 2: Place start ('S') and end ('E')
    placeStartAndEnd(maze);

    // Step 3: Render the maze to the display
    renderMaze(maze);

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
    for (int row = 0; row < maze.size(); ++row) {
        for (int col = 0; col < maze[row].size(); ++col) {
            drawElement(row, col, maze[row][col]);
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