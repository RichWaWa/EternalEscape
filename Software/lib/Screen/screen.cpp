#include "screen.h"

//Constructor
ScreenManager::ScreenManager() 
  //Member initializer list
  : tft(TFT_CS, TFT_DC, TFT_RST),  // Initialize tft with required pin values
    ts(TS_XP, TS_YP, TS_XM, TS_YM, TS_RES) {  // Initialize ts with required pin values and resistance
  // Any additional initialization if needed
}

//Initialize Display
void ScreenManager::init() {
  tft.begin(SPI_FREQUENCY);
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
}

//Get touch points
void ScreenManager::getTouchPoints(int16_t& x, int16_t& y, int16_t& z) {
  TSPoint touchPoint = ts.getPoint();
  x = map(touchPoint.x, TS_MINX, TS_MAXX, 0, tft.width());
  y = map(touchPoint.y, TS_MINY, TS_MAXY, 0, tft.height());
  z = abs(touchPoint.z);
  Serial.print("Touch detected at: ");
  //Serial.print("X=");
  //Serial.print(x);
  //Serial.print(", Y=");
  //Serial.print(y);
  Serial.print(", Z=");
  Serial.println(z);
}

//Draw Strobe Screen
void ScreenManager::drawStrobeScreen(uint16_t color) {
  tft.fillScreen(color);
}

// Draw the settings screen
void ScreenManager::drawSettingsScreen(const String& macAddress) {
  tft.fillScreen(ILI9341_BLACK);

  // Draw title
  drawTextCentered("EternalEscape", tft.width() / 2, 10, ILI9341_WHITE);

  // Draw MAC address
  drawText("MAC Address:", 10, 40, ILI9341_WHITE);
  drawText(macAddress.c_str(), 10, 60, ILI9341_WHITE);

  // Draw brightness toggle
  drawText("Brightness:", 10, 100, ILI9341_WHITE);
  drawRectangle(120, 90, 60, 30, ILI9341_WHITE);
  drawTextCentered(brightnessLevel.c_str(), 150, 105, ILI9341_BLACK);
}

// Toggle brightness levels
void ScreenManager::toggleBrightness() {
  if (brightnessLevel == "HI") {
    brightnessLevel = "MD";
  } else if (brightnessLevel == "MD") {
    brightnessLevel = "LO";
  } else {
    brightnessLevel = "HI";
  }
}

const String& ScreenManager::getBrightnessLevel() const {
  return brightnessLevel;
}

// Utility functions
void ScreenManager::drawText(const char* text, int16_t x, int16_t y, uint16_t color) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(2);
  tft.print(text);
}

void ScreenManager::drawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  tft.drawRect(x, y, w, h, color);
}

void ScreenManager::drawTextCentered(const char* text, int16_t centerX, int16_t centerY, uint16_t color) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor(centerX - (w / 2), centerY - (h / 2));
  tft.setTextColor(color);
  tft.setTextSize(2);
  tft.print(text);
}
