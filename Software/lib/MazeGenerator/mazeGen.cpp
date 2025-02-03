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
    drawFillScreen(DARKGREY); //fill orange
    //start at row one to exclude the border
    for (int row = 1; row < maze.size() - 1; ++row) {
        for (int col = 1; col < maze[row].size() - 1; ++col) {
            drawElement(row, col, maze[row][col]);
        }
    }
}


