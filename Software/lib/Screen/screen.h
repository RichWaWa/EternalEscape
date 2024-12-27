#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_ILI9341.h>
#include "TouchScreen.h"

// TFT display pin definitions
#define TFT_CS 9               // Chip select control pin
#define TFT_DC 6               // Data Command control pin
#define TFT_RST 5              // Reset pin (could connect to RST pin)
#define SPI_FREQUENCY 40000000 // 40 MHz

// Touchscreen pin definitions
#define TS_YP 14               // Y+ pin (must be an analog pin)
#define TS_XP 15               // X+ pin
#define TS_YM 16               // Y- pin
#define TS_XM 17               // X- pin (must be an analog pin)

// Touchscreen calibration values
//TODO Figure out what these actually do.
#define TS_MINX 150            // 150
#define TS_MINY 120            // 120
#define TS_MAXX 920            // 920
#define TS_MAXY 940            // 940
#define TS_RES 332             // Touch screen resistance (ADJUST FOR YOUR DISPLAY, But this value may be close enough!)
// define valid pressure ranges to eliminate error
// NOTE These will likely need to be adjusted for your display!!!
#define MINPRESSURE 100
#define MAXPRESSURE 10000

class ScreenManager {
public:
    ScreenManager();
    void init();
    void getTouchPoints(int16_t& x, int16_t& y, int16_t& z);
    void drawStrobeScreen(uint16_t color);
    void drawSettingsScreen(const String& macAddress);
    void toggleBrightness();
    const String& getBrightnessLevel() const;

private:
    Adafruit_ILI9341 tft;
    TouchScreen ts;
    String brightnessLevel;

    void drawText(const char* text, int16_t x, int16_t y, uint16_t color);
    void drawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawTextCentered(const char* text, int16_t centerX, int16_t centerY, uint16_t color);
};

#endif // SCREEN_H
