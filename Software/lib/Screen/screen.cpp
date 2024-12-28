#include "screen.h"
#include "settings.h" //Settings are only accessed by sub functions.

// Global variables for TFT and touchscreen
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);  // Initialize tft with required pin values
TouchScreen ts(TS_XP, TS_YP, TS_XM, TS_YM, TS_RES);  // Initialize ts with required pin values and resistance
String brightnessLevel = "HI";      // Default brightness level
int brightnessPWM = 255;              //Default brightness PWM
bool wifiStatusLast = false;        //tracks whether the wifi status has changed or not
String brightnessLevelLast = "";

// Initialize Display
void initScreen() {
    tft.begin(SPI_FREQUENCY);
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
}

// Draw the settings screen
void drawSettingsScreen(const String& macAddress, bool wifiStatus) {
    tft.fillScreen(ILI9341_BLACK);

    // Draw title
    drawTextCentered("EternalEscape", tft.width() / 2, 10, ILI9341_WHITE);

    // Draw wifi status
    if (wifiStatus) {
        drawText("Wifi Connected", 10, 40, ILI9341_GREEN);
    } else {
        drawText("Wifi Disconnected", 10, 40, ILI9341_RED);
    }
    wifiStatusLast = wifiStatus;    //update the wifi status
    
    // Draw MAC address
    drawText("MAC Address:", 10, 60, ILI9341_WHITE);
    drawText(macAddress.c_str(), 10, 80, ILI9341_WHITE);

    // Draw brightness toggle
    brightnessLevel = loadBrightness(); //load the brightness from the settings
    drawText("Brightness:", 10, 140, ILI9341_WHITE);
    drawFillRectangle(180, 140, 60, 30, ILI9341_WHITE);  // Outline rectangle
    drawTextCentered(brightnessLevel.c_str(), 210, 155, ILI9341_BLACK);
    brightnessLevelLast = brightnessLevel;
}

// Update the settings screen (partial updates only)
void updateSettingsScreen(bool wifiStatus) {
    // Update wifi status only if it has changed
    if (wifiStatus != wifiStatusLast) {
        if (wifiStatus) {
            drawText("Wifi Connected", 10, 40, ILI9341_GREEN);
        } else {
            drawText("Wifi Disconnected", 10, 40, ILI9341_RED);
        }
        wifiStatusLast = wifiStatus;
    }

    // Update brightness level only if it has changed
    if (brightnessLevel != brightnessLevelLast) {
        drawFillRectangle(180, 140, 60, 30, ILI9341_BLACK);  // Clear the previous rectangle
        drawFillRectangle(180, 140, 60, 30, ILI9341_WHITE);      // Redraw outline rectangle
        drawTextCentered(brightnessLevel.c_str(), 210, 155, ILI9341_BLACK);
        brightnessLevelLast = brightnessLevel;
    }
}

// Get touch points
void getTouchPoints(int16_t& x, int16_t& y, int16_t& z) {
    TSPoint touchPoint = ts.getPoint();
    x = map(touchPoint.x, TS_MINX, TS_MAXX, 0, tft.width());
    y = map(touchPoint.y, TS_MINY, TS_MAXY, 0, tft.height());
    z = abs(touchPoint.z);
    Serial.print("Touch detected at: ");
    //Serial.print("X=");
    //Serial.print(x);
    //Serial.print(", Y=");
    //Serial.print(y);
    //Serial.print(", Z=");
    //Serial.println(z);
}

// Draw Strobe Screen
void drawStrobeScreen(uint16_t color) {
    tft.fillScreen(color);
}

// Toggle brightness levels
void toggleBrightness() {
    if (brightnessLevel == "HI") {
        brightnessLevel = "MD";
        brightnessPWM = 170;    // 2/3 of 255
    } else if (brightnessLevel == "MD") {
        brightnessLevel = "LO";
        brightnessPWM = 85;     // 1/3 of 255
    } else {
        brightnessLevel = "HI";
        brightnessPWM = 255;    // 3/3 of 255
    }
    saveBrightness(brightnessLevel);
}

// Get the current brightness level
const int getBrightnessPWM() {
    return brightnessPWM;
}

// Utility functions
void drawText(const char* text, int16_t x, int16_t y, uint16_t color) {
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.print(text);
}

void drawFillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    tft.fillRect(x, y, w, h, color);
}

void drawTextCentered(const char* text, int16_t centerX, int16_t centerY, uint16_t color) {
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    tft.setCursor(centerX - (w / 2), centerY - (h / 2));
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.print(text);
}
