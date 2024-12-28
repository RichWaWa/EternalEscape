#include "screen.h"
#include "settings.h" //Settings are only accessed by sub functions.

// Global variables for TFT and touchscreen
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);  // Initialize tft with required pin values
TouchScreen ts(TS_XP, TS_YP, TS_XM, TS_YM, TS_RES);  // Initialize ts with required pin values and resistance
String brightnessLevel = "HI";      // Default brightness level
int brightnessPWM = 255;            //Default brightness PWM
bool wifiStatusLast = false;        //tracks whether the wifi status has changed or not
String brightnessLevelLast = "";    
// Debounce tracking variables
unsigned long lastTouchTime = 0;
const unsigned long debounceDelay = 300; // 300ms debounce delay

// Initialize Display
void initScreen() {
    tft.begin(SPI_FREQUENCY);
    tft.setRotation(3); //sets the correct screen orientation. (SD card facing down. If you didnt do this, set it to 1)
    tft.fillScreen(ILI9341_BLACK);  
    pinMode(TFT_LITE, OUTPUT); //set the PWM output for the backlight
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
    checkBrightnessButtonTouch(0, 0, 0); //I use this function to just load the box initially. Setting the arguments to just zeros so it doesnt do anything
    brightnessLevelLast = brightnessLevel;
}

// Update the settings screen (partial updates only)
void updateSettingsScreen(int16_t x, int16_t y, int16_t z ,bool wifiStatus) {
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
        checkBrightnessButtonTouch(x, y, z);
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

// Check if the brightness button has been toggled
void checkBrightnessButtonTouch(int16_t x, int16_t y, int16_t z) {
    // Define rectangle bounds
    const int16_t rectX = 180;   // X-coordinate of the rectangle
    const int16_t rectY = 140;   // Y-coordinate of the rectangle
    const int16_t rectWidth = 60;  // Width of the rectangle
    const int16_t rectHeight = 30; // Height of the rectangle

    // Check for valid touch input
    if (z > MINPRESSURE && z < MAXPRESSURE) {
        // Check if the touch is within the rectangle bounds
        if (x >= rectX && x <= (rectX + rectWidth) &&
            y >= rectY && y <= (rectY + rectHeight)) {
            
            // Check for debounce
            unsigned long currentTime = millis();
            if (currentTime - lastTouchTime > debounceDelay) {
                // Toggle brightness level
                toggleBrightness(); // Call your brightness toggle function here

                // Update the brightness level on the screen
                drawFillRectangle(rectX, rectY, rectWidth, rectHeight, ILI9341_WHITE); // Redraw rectangle
                drawTextCentered(brightnessLevel.c_str(), rectX + (rectWidth / 2), rectY + (rectHeight / 2), ILI9341_BLACK);

                // Update debounce timer
                lastTouchTime = currentTime;
            }
        }
    }
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
    //saveBrightness(brightnessLevel);    //save to settings
    analogWrite(TFT_LITE, brightnessPWM);   //set brightness level
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
