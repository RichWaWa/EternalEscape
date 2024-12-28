#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

#include <Preferences.h> // Include the Preferences library

// Initialize the preferences storage
void initializePreferences();

// Save the brightness level to EEPROM
void saveBrightness(String brightness);

// Load the brightness level from EEPROM
String loadBrightness();

// Clear all stored settings (optional, for reset purposes)
void clearPreferences();

#endif // EEPROM_UTILS_H
