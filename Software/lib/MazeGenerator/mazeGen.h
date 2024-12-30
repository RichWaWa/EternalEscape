#ifndef MAZEGEN_H
#define MAZEGEN_H
#include <vector>
using namespace std;

// Declare the maze template //This one has dimentions [31][41]
vector<vector<char>>  mazeTemplate = {
    // Top border
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '#', 'C', '#', 'C', '#', 'C', '#', 'c', '#', 'c', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', 'C', '#', '*'},
    {'*', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '#', '.', '*'},
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}
};//end mazeTemplate


//maze generator functions
void generateMaze();        //Generate the maze
void drawElement(int rowA, int colA, char cellType);   //draw the cell element
//void drawMazeTemplate();

#endif // MAZEGEN_H
