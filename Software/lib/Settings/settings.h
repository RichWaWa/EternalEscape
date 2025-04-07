#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

#include <Preferences.h> // Include the Preferences library
#include <Arduino.h>
#include <String.h>

// Initialize preferences
void initializePreferences();

// Brightness functions
void saveBrightness(int brightness);
int loadBrightness();

// Player Speed functions
void savePlayerSpeed(int speed);
int loadPlayerSpeed();

// Maze Speed functions
void saveMazeSpeed(int speed);
int loadMazeSpeed();

// Victory Timeout functions
void saveVictoryTimeout(int timeout);
int loadVictoryTimeout();

// Player 2 toggle functions
void savePlayer2(bool enabled);
bool loadPlayer2();

// Clear all preferences
void clearPreferences();

#endif
