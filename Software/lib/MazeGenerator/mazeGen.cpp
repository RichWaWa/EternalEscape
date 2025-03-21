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
//the maze which will be built
vector<vector<char>>  maze;

// Directions for movement (up, down, left, right)
const vector<pair<int, int>> directions = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};

//start and end global variables
vector<pair<int, int>> startPositions = {{0, 0}};
int endRow = 0, endCol = 0;
//render speed
const int speedMS = 10; //was 70

// Prim's Algorithm to generate the maze
void generateMaze() {
    //Copy the template
    maze = copyMazeTemplate();
    //Generate start ('S') and end ('E')
    generateStartAndEnd(maze);
    //Render the initial maze template to the display
    renderMaze(maze);
    srand(time(0));  // Seed random number generator
    // Initialize frontier list with walls adjacent to start
    vector<pair<int, int>> frontier;
    //Create the initial frontier cells
    for (size_t i = 0; i < directions.size(); i++) {
        //Serial.println("Creating Initial Frontier");
        int dr = directions[i].first;
        int dc = directions[i].second;

        int newRow = startPositions[0].first + dr;
        int newCol = startPositions[0].second + dc;

        if (isValidCell(newRow, newCol, '.', maze)) { //check if the cell is a valid path
            frontier.push_back({newRow, newCol}); // Add cell to frontier
            drawElement(newRow, newCol, 'F'); // Draw in LIGHTRED
            delay(speedMS);
        }
    }

    // Process frontier list
    while (!frontier.empty()) {
        //Serial.println("Processing Frontier list!");
        // Pick a random frontier cell
        int randIndex = rand() % frontier.size();
        //extract values from frontier vector
        int cellRow = frontier[randIndex].first;
        int cellCol = frontier[randIndex].second;
        frontier.erase(frontier.begin() + randIndex);

        // Check neighbors to connect to the maze
        vector<pair<int, int>> neighbors;
        for (size_t i = 0; i < directions.size(); i++) {
            int dr = directions[i].first;
            int dc = directions[i].second;
            int newRow = cellRow + dr;
            int newCol = cellCol + dc;
            if (isValidCell(newRow, newCol, '0', maze)) {
                neighbors.push_back({newRow, newCol});
                //Serial.println("Neighbor Found");
            }
        }

        if (!neighbors.empty()) {
            // Choose a random connected path cell
            int randIndex = rand() % neighbors.size();
            int neighborRow = neighbors[randIndex].first;
            int neighborCol = neighbors[randIndex].second;

            // Remove the wall between them and set to black.
            int wallRow = (cellRow + neighborRow) / 2;
            int wallCol = (cellCol + neighborCol) / 2;
            maze[wallRow][wallCol] = ','; //mark as a path where a former wall was
            drawElement(wallRow, wallCol, ','); // Black
            //Serial.println("Removing Wall!!");

            // Mark new cell as part of the maze
            maze[cellRow][cellCol] = '0';
            drawElement(cellRow, cellCol, '0'); // Black

            // Add new frontier cells
           for (size_t i = 0; i < directions.size(); i++) {
                int dr = directions[i].first;
                int dc = directions[i].second;

                int newRow = cellRow + dr;
                int newCol = cellCol + dc;

                if (isValidCell(newRow, newCol, '.', maze) && !inFrontier(newRow, newCol, frontier)) {
                    frontier.push_back({newRow, newCol}); // Add cell to frontier
                    drawElement(newRow, newCol, 'F'); // Draw in LIGHTRED
                    delay(speedMS); //100ms delay
                }
            }
            delay(speedMS); //100ms delay
        }
    }

    // Ensure 'E' remains unchanged
    drawElement(endRow, endCol, 'E'); // Checkered finish
    maze[endRow][endCol] = 'E';
    Serial.println("Maze Drawn");
}

// Function to copy the template
vector<vector<char>> copyMazeTemplate() {
    return mazeTemplate;
}

// Getter for the maze
vector<vector<char>> mazeGetter() {
    return maze;
}

vector<pair<int, int>> getStartPositions()
{
    return startPositions;
}

// Function to check if a cell is a valid candidate for maze expansion
//The isValidCell function checks if we are in bounds, AND if the element in question is the one we are looking for at row, col
bool isValidCell(int row, int col, char element, const vector<vector<char>>& maze) {
    // First, check if the row and column are within valid bounds.
    if (row <= 0 || row >= maze.size() - 1 || col <= 0 || col >= maze[0].size() - 1) {
        return false;
    }
    // Now it's safe to access maze[row][col]
    return (maze[row][col] == element);
}

// Check if a proposed frontier cell is already in the frontier.
bool inFrontier(int row, int col, const vector<pair<int, int>>& frontier) {
    for (size_t i = 0; i < frontier.size(); i++) {
        if (frontier[i].first == row && frontier[i].second == col) {
            return true;
        }
    }
    return false;
}

// Function to choose a random "#" for start ('S') and end ('E')
void generateStartAndEnd(vector<vector<char>>& maze) {
    int rows = maze.size();
    int cols = maze[0].size();

    srand(time(0)); // Seed for random number generation

    // Randomly select start
    bool startPlaced = false;
    while (!startPlaced) {
        int row = 1 + rand() % (rows - 2); // rows 1 to rows-2
        int col = 1 + rand() % (cols - 2); // cols 1 to cols-2
        if (maze[row][col] == '.') {
            //save start pos to global vars
            startPositions[0].first  = row;
            startPositions[0].second = col;
            // Mark new cell as part of the maze
            maze[startPositions[0].first][startPositions[0].second] = '0';
            startPlaced = true;
        }
    }

    // Randomly select end
    bool endPlaced = false;
    while (!endPlaced) {
        int row = 1 + rand() % (rows - 2);
        int col = 1 + rand() % (cols - 2);
        if (maze[row][col] == '.' && (row != startPositions[0].first || col != startPositions[0].second)) {
            //save end pos to global vars
            endRow = row;
            endCol = col;
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


