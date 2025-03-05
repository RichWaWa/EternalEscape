#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <vector>
using namespace std;

const vector<pair<int, int>> pathDirections = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
const vector<pair<int, int>> wallDirections = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

void randRecursiveMazeSolver(vector<vector<char>>& maze, vector<pair<int, int>>& position, const char playerChar, const char playerPathChar);
bool isValidMove(int newRow, int newCol, int wallRow, int wallCol, const vector<vector<char>>& maze);
void printDebugInfo(const char* message, int row = -1, int col = -1);

#endif

