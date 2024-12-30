#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_ILI9341.h>
#include "TouchScreen.h"

// TFT display pin definitions
#define TFT_LITE 10           // Backlight brightness control pin (Drive with PWM)
#define TFT_CS 9               // Chip select control pin
#define TFT_DC 6               // Data Command control pin
#define TFT_RST 5              // Reset pin (could connect to RST pin)
#define SPI_FREQUENCY 40000000 // 40 MHz

//TFT Display Size
#define TFT_WIDTH 320
#define TFT_HEIGHT 240

// Touchscreen pin definitions
#define TS_YP 14               // Y+ pin (must be an analog pin)
#define TS_XP 15               // X+ pin
#define TS_YM 16               // Y- pin
#define TS_XM 17               // X- pin (must be an analog pin)

// Touchscreen calibration values
//These are needed to map the calibration values.
//You WILL need to adjust these for your screen. See WIKI
#define TS_MINX 806            // Default 150
#define TS_MINY 810            // Default 120
#define TS_MAXX 972            // Default 920
#define TS_MAXY 983            // Default 940
#define TS_RES 332             // Touch screen resistance (ADJUST FOR YOUR DISPLAY, But this value may be close enough!)
// define valid pressure ranges to eliminate error
// NOTE These will likely need to be adjusted for your display!!!
#define MINPRESSURE 150
#define MAXPRESSURE 3000

// Initialization functions
void initScreen();
void getTouchPoints(int16_t& x, int16_t& y, int16_t& z);

// Screen drawing functions
void drawStrobeScreen(uint16_t color);
void drawSettingsScreen(const String& macAddress, bool wifiStatus);
void updateSettingsScreen(int16_t x, int16_t y, int16_t z, bool wifiStatus);
void checkBrightnessButtonTouch(int16_t x, int16_t y, int16_t z);
void loadingScreen();

// Helper functions
void drawText(const char* text, int16_t x, int16_t y, uint16_t color);
void drawFillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawFillScreen(uint16_t color);
void drawBorderedRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t borderColor, uint16_t fillColor, uint16_t t);
void drawTextCentered(const char* text, int16_t centerX, int16_t centerY, uint16_t color);
void toggleBrightness();
String getBrightnessLevel();

#endif // SCREEN_H
