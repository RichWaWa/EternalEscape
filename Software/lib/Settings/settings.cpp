#include "settings.h"
//See https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html

#include "settings.h"

// Create a global Preferences object
Preferences preferences;

// Function to initialize preferences
void initializePreferences() {
    preferences.begin("settings", false); // Namespace "settings", RW mode

    // Check if this is the first run by looking for a "firstRun" key
    if (!preferences.isKey("firstRun")) {
        // Set default values for all settings
        preferences.putString("brightness", "HI");   // Default brightness level
        preferences.putInt("playerSpeed", 100);        // Default player move speed
        preferences.putInt("mazeSpeed", 10);           // Default maze generation speed
        preferences.putInt("victoryTimeout", 4000);      // Default victory timeout
        preferences.putBool("player2", false);         // Default player2 disabled

        preferences.putBool("firstRun", true);         // Mark first run as complete
    }
    preferences.end();
}

// Function to save the brightness level (as int)
void saveBrightness(int brightness) {
    preferences.begin("settings", false);
    preferences.putInt("brightness", brightness);
    preferences.end();
}

// Function to load the brightness level (returns int)
int loadBrightness() {
    preferences.begin("settings", true);
    int brightness = 255; // Default to full brightness
    if (preferences.isKey("brightness")) {
        brightness = preferences.getInt("brightness");
    }
    preferences.end();
    return brightness;
}

// Function to save the player speed (as int)
void savePlayerSpeed(int speed) {
    preferences.begin("settings", false);
    preferences.putInt("playerSpeed", speed);
    preferences.end();
}

// Function to load the player speed (returns int)
int loadPlayerSpeed() {
    preferences.begin("settings", true);
    int speed = 100; // Default player speed
    if (preferences.isKey("playerSpeed")) {
        speed = preferences.getInt("playerSpeed");
    }
    preferences.end();
    return speed;
}

// Function to save the maze speed (as int)
void saveMazeSpeed(int speed) {
    preferences.begin("settings", false);
    preferences.putInt("mazeSpeed", speed);
    preferences.end();
}

// Function to load the maze speed (returns int)
int loadMazeSpeed() {
    preferences.begin("settings", true);
    int speed = 10; // Default maze speed
    if (preferences.isKey("mazeSpeed")) {
        speed = preferences.getInt("mazeSpeed");
    }
    preferences.end();
    return speed;
}

// Function to save the victory timeout (as int)
void saveVictoryTimeout(int timeout) {
    preferences.begin("settings", false);
    preferences.putInt("victoryTimeout", timeout);
    preferences.end();
}

// Function to load the victory timeout (returns int)
int loadVictoryTimeout() {
    preferences.begin("settings", true);
    int timeout = 4000; // Default victory timeout
    if (preferences.isKey("victoryTimeout")) {
        timeout = preferences.getInt("victoryTimeout");
    }
    preferences.end();
    return timeout;
}

// Function to save the Player 2 toggle (as bool)
void savePlayer2(bool enabled) {
    preferences.begin("settings", false);
    preferences.putBool("player2", enabled);
    preferences.end();
}

// Function to load the Player 2 toggle (returns bool)
bool loadPlayer2() {
    preferences.begin("settings", true);
    bool enabled = false; // Default: player2 disabled
    if (preferences.isKey("player2")) {
        enabled = preferences.getBool("player2");
    }
    preferences.end();
    return enabled;
}

// Function to clear all preferences (if needed)
void clearPreferences() {
    preferences.begin("settings", false);
    preferences.clear();
    preferences.end();
}
