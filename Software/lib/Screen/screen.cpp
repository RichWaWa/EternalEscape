#include "screen.h"

//Constructor
ScreenManager::ScreenManager() 
  //Member initializer list
  : tft(TFT_CS, TFT_DC, TFT_RST),  // Initialize tft with required pin values
    ts(TS_XP, TS_YP, TS_XM, TS_YM, TS_RES) {  // Initialize ts with required pin values and resistance
  // Any additional initialization if needed
}

void ScreenManager::init() {
  tft.begin(SPI_FREQUENCY);
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
}

void ScreenManager::drawFillScreen(uint16_t color) {
  tft.fillScreen(color);
}

void ScreenManager::getTouchPoints(int16_t& x, int16_t& y, int16_t& z) {
  TSPoint touchPoint = ts.getPoint();
  x = map(touchPoint.x, TS_MINX, TS_MAXX, 0, tft.width());
  y = map(touchPoint.y, TS_MINY, TS_MAXY, 0, tft.height());
  z = abs(touchPoint.z);
  //Serial.print("Touch detected at: ");
  //Serial.print("X=");
  //Serial.print(x);
  //Serial.print(", Y=");
  //Serial.print(y);
  //Serial.print(", Z=");
  //Serial.println(z);
}

//TODO add main loop that runs on different thread for GUI handling?
