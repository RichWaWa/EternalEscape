#include "settings.h"
//See https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html

// Create a global Preferences object
Preferences preferences;

// Function to initialize preferences
void initializePreferences() {
    preferences.begin("settings", false); // Namespace "settings", RW mode

    // Check if this is the first run by looking for a "firstRun" key
    if (!preferences.isKey("firstRun")) {
        //Serial.println("I think its my first run");    //debug
        // Set default values for all settings
        preferences.putString("brightness", "HI"); // Default brightness
        preferences.putBool("firstRun", false); // Mark first run as complete
    }
    preferences.end();
}

// Function to save the brightness level
void saveBrightness(String brightness) {
    preferences.begin("settings", false);// Namespace "settings", RW mode

    preferences.putString("brightness", brightness); // Store the brightness value
    //Serial.print("saved brightness");    //debug
    //Serial.println(brightness);

    preferences.end();
}

// Function to load the brightness level
String loadBrightness() {
    preferences.begin("settings", true); // Open Preferences in read mode
    String brightness = "HI"; // Default value

    if (preferences.isKey("brightness")) {
        //Serial.println("Found the brightness setting!");
        brightness = preferences.getString("brightness"); // Retrieve stored value
    } else {
        //Serial.println("Brightness not found, using default value.");
    }

    preferences.end(); // Close Preferences
    return brightness;
}

// Function to clear all preferences
void clearPreferences() {
    preferences.begin("settings", false); // Open Preferences
    preferences.clear(); // Erase all keys in the current namespace
    preferences.end(); // Close Preferences
}
