#include "settings.h"
//See https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html

// Create a global Preferences object
Preferences preferences;

// Function to initialize preferences
void initializePreferences() {
    preferences.begin("settings", false); // Namespace "settings", RW mode

    // Check if this is the first run by looking for a "firstRun" key
    if (!preferences.isKey("firstRun")) {
        // Set default values for all settings
        preferences.putString("brightness", "HI"); // Default brightness
        preferences.putBool("firstRun", false); // Mark first run as complete
    }
}

// Function to save the brightness level
void saveBrightness(String brightness) {
    preferences.putString("brightness", brightness); // Store the brightness value
}

// Function to load the brightness level
String loadBrightness() {
    if (preferences.isKey("brightness")) {
        return preferences.getString("brightness"); // Retrieve the brightness value
    } else {
        return "HI"; // Default brightness level if not set
    }
}

// Function to clear all preferences
void clearPreferences() {
    preferences.clear(); // Erase all keys in the current namespace
}
