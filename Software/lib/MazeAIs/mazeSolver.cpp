#include "mazeSolver.h"
#include "screen.h"  // For drawElement and delay

vector<pair<int, int>> randRecursiveMazeSolver(const vector<vector<char>>& maze, vector<pair<int, int>> position){
    // Generate a random number between 0 and 3

    //As it moves along, it should mark its trail with an x. 
    //Each time it pops back to backtrack, it should also be marked with an x so it doesnt attempt to go there again.
    // additionally, it should randomly decide a direction to go. 
    //it needs to check if the directly adjacent positions are walls, and if there is not a wall, it needs to check if it has already gone to that path
    // Lastly, it should return the position that it wants to go as a vector pair.
    int randomNumber = rand() % 4;


}

