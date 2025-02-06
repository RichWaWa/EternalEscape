#include "screen.h"
#include "settings.h" //Settings are only accessed by sub functions.

// Global variables for TFT and touchscreen
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);  // Initialize tft with required pin values
TouchScreen ts(TS_XP, TS_YP, TS_XM, TS_YM, TS_RES);  // Initialize ts with required pin values and resistance
//Brightness button Variables
const int16_t rectX = 150;   // X-coordinate of the rectangle
const int16_t rectY = 133;   // Y-coordinate of the rectangle
const int16_t rectWidth = 60;  // Width of the rectangle
const int16_t rectHeight = 30; // Height of the rectangle

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
    //need to load the brightness from the settings
    brightnessPWM = loadBrightnessFromSettings();
    analogWrite(TFT_LITE, brightnessPWM);   //set brightness level
    loadingScreen();    //show the loading screen

}

// Draw the settings screen
void drawSettingsScreen(const String& macAddress, bool wifiStatus) {
    delay(10);  //Small delay to fix issue with Buffer not initializing correctly
    tft.fillScreen(ILI9341_BLACK);

    // Draw title
    drawTextCentered("EternalEscape", (tft.width() / 2), 10, ILI9341_WHITE);

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
    drawText("Brightness:", 10, 140, ILI9341_WHITE);
    drawFillRectangle(rectX, rectY, rectWidth, rectHeight, ILI9341_WHITE); //draw button
    drawTextCentered(brightnessLevel.c_str(), rectX + (rectWidth / 2), rectY + (rectHeight / 2), ILI9341_BLACK);//add text
    brightnessLevelLast = brightnessLevel;
}

// Update the settings screen (partial updates only)
void updateSettingsScreen(int16_t x, int16_t y, int16_t z ,bool wifiStatus) {
    // Update wifi status only if it has changed
    if (wifiStatus != wifiStatusLast) {
        int16_t x1, y1;
        uint16_t w, h;
        if (wifiStatus) {
            drawText("Wifi Connected", 10, 40, ILI9341_GREEN);
            tft.getTextBounds("Wifi Connected", 0, 0, &x1, &y1, &w, &h);
            tft.fillRect(x1, y1, w, h, ILI9341_BLACK);
        } else {
            drawText("Wifi Disconnected", 10, 40, ILI9341_RED);
            tft.getTextBounds("Wifi Disconnected", 0, 0, &x1, &y1, &w, &h);
            tft.fillRect(x1, y1, w, h, ILI9341_BLACK);
        }
        wifiStatusLast = wifiStatus;
    }

    // Update brightness level only if it has changed
    checkBrightnessButtonTouch(x, y, z);
    if (brightnessLevel != brightnessLevelLast) {
        brightnessLevelLast = brightnessLevel;
    }
}

// Get touch points
void getTouchPoints(int16_t& x, int16_t& y, int16_t& z) {
    TSPoint touchPoint = ts.getPoint();
    // Map x and y to origin at top left corner.
    x = map(touchPoint.x, TS_MINY, TS_MAXY, 0, tft.width());        // X-axis: downwards
    y = map(touchPoint.y, TS_MINX, TS_MAXX, 0, tft.height()); 
    z = abs(touchPoint.z);
    y = tft.height() - y;   // Adjust Y-axis to invert it, leaving  Y-axis: rightwards

    //Uncomment these lines to read the touchscreen printouts!
    //Serial.print("Touch detected at: ");
    //Serial.print("X=");
    //Serial.print(touchPoint.x);
    //Serial.print(", Y=");
    //Serial.print(touchPoint.y);
    //Serial.print(", Z=");
    //Serial.println(touchPoint.z);

    //These lines are to confirm the mapped touch points
    //Serial.print("Mapped Touch detected at: ");
    //Serial.print("X=");
    //Serial.print(x);
    //Serial.print(", Y=");
    //Serial.print(y);
    //Serial.print(", Z=");
    //Serial.println(z);
}

// Check if the brightness button has been toggled
void checkBrightnessButtonTouch(int16_t x, int16_t y, int16_t z) {
    // Check for valid touch input
    if (z > MINPRESSURE && z < MAXPRESSURE) {
        // Check if the touch is within the rectangle bounds
        if (x >= rectX && x <= (rectX + rectWidth) &&
            y >= rectY && y <= (rectY + rectHeight)) {
            // Check for debounce
            unsigned long currentTime = millis();
            if (currentTime - lastTouchTime > debounceDelay) {
                // Toggle brightness level
                toggleBrightness(); // toggle brightness

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

void loadingScreen() {
    // Clear the screen
    tft.fillScreen(ILI9341_BLACK);

    // Draw "EternalEscape" text
    tft.setTextSize(4);
    tft.setFont();
    tft.setTextColor(ILI9341_WHITE);
    drawTextCentered("EternalEscape", tft.width() / 2, tft.height() / 2 - 20, ILI9341_WHITE);

    // Draw "Loading..." text
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_DARKGREY);
    drawTextCentered("Loading...", tft.width() / 2, tft.height() / 2 + 20, ILI9341_DARKGREY);
}


// Toggle brightness levels
void toggleBrightness() {
    if (brightnessLevel.equals("HI")) {
        brightnessLevel = "MD";
        brightnessPWM = 170;    // 2/3 of 255
    } else if (brightnessLevel.equals("MD")) {
        brightnessLevel = "LO";
        brightnessPWM = 85;     // 1/3 of 255
    } else {
        brightnessLevel = "HI";
        brightnessPWM = 255;    // 3/3 of 255
    }
    Serial.println(brightnessLevel);    //Debug
    saveBrightness(brightnessLevel);    //save to settings
    analogWrite(TFT_LITE, brightnessPWM);   //set brightness level
}

//load and apply brightness from settings
int loadBrightnessFromSettings(){
    brightnessLevel = loadBrightness(); //load from settings
    //Serial.print("Here is the brightness I loaded ");
    //Serial.println(brightnessLevel);

    if (brightnessLevel.equals("LO")) {
        brightnessPWM = 85;     // 1/3 of 255
    } else if (brightnessLevel.equals("MD")) {
        brightnessPWM = 170;    // 2/3 of 255
    } else {
        brightnessLevel = "HI";
        brightnessPWM = 255;    // 3/3 of 255
        //Serial.println("Load brightness from settings defaulted");
    }
    return brightnessPWM;
}
////////////////////
// Utility functions
////////////////////
void drawText(const char* text, int16_t x, int16_t y, uint16_t color) {
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.print(text);
}

void drawFillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    tft.fillRect(x, y, w, h, color);
}

void drawFillScreen(uint16_t color){
    tft.fillScreen(color);
}

//draw a border rectangle.
void drawBorderedRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t borderColor, uint16_t fillColor, uint16_t t) 
{
    // Draw the outer rectangle (the border)
    tft.fillRect(x, y, w, h, borderColor);

    // Draw the inner rectangle (the fill), offset by thickness t
    if (t < w / 2 && t < h / 2) { // Ensure thickness is valid
        tft.fillRect(x + t, y + t, w - 2 * t, h - 2 * t, fillColor);
    }
}

//draws centered text
void drawTextCentered(const char* text, int16_t centerX, int16_t centerY, uint16_t color) {
    int16_t x1, y1;
    uint16_t w, h;
    tft.setTextSize(2); // Set the text first to ensure getTextBounds returns the correct value
    tft.setFont();      // Ensure the default font is selected
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    tft.setCursor(centerX - (w / 2), centerY - (h / 2));
    tft.setTextColor(color);
    tft.print(text);
}

//draw finish square
void drawFinishSquare(int16_t x, int16_t y, int16_t cellSize){
    int16_t halfCell = cellSize / 2; // Calculate half of the cell size for rows and columns

    // Draw the black background
    drawFillRectangle(x, y, cellSize, cellSize, ILI9341_BLACK); // Black background

    // Draw the white checkered pattern
    for (int16_t row = 0; row < cellSize; row += 4) { // Step by 4 pixels (2 for each white square)
        for (int16_t col = 0; col < cellSize; col += 4) {
            // Top-left square of the 4x4 grid
            drawFillRectangle(x + col, y + row, 2, 2, ILI9341_WHITE); // White square
            // Bottom-right square of the 4x4 grid
            if (col + 2 < cellSize && row + 2 < cellSize) {
                drawFillRectangle(x + col + 2, y + row + 2, 2, 2, ILI9341_WHITE); // White square
            }
        }
    }
}

//Call this to draw the maze elements
void drawElement(int rowA, int colA, char cellType) {
    // Maze cell dimensions
    const int cellSize = 14;
    const int wallThickness = 2;
    const int offset = 1; // Account for the 1-pixel border around the maze area (temp changed to 0)

    // Calculate the top-left pixel coordinates of the cell
    //needs to accomodate that the cells are different sizes!
    int x = offset + (colA - 1) * ((cellSize + wallThickness) /2); //this has to be 8 for some reason. Some relationship between the cell size and the wall thickness
    int y = offset + (rowA - 1) * ((cellSize + wallThickness) /2);

    // Determine what to draw based on cellType
    switch (cellType) {
        case '.': // Path cell (black 14x14)
            drawFillRectangle(x, y, cellSize, cellSize, BLACK);
            break;

        case ',': // Wall path cell
            if (colA % 2 == 1 && rowA % 2 == 0) {
                // Odd column, even row: Horizontal wall
                drawFillRectangle(x, y + (cellSize / 2 - 1), cellSize, wallThickness, BLACK);
            } else if (colA % 2 == 0 && rowA % 2 == 1) {
                // Even column, odd row: Vertical wall
                drawFillRectangle(x + (cellSize / 2 - 1), y, wallThickness, cellSize, BLACK);
            } else {
                // Default fallback for unexpected wall cases
                drawFillRectangle(x, y, 4, 4, GREEN);
            }
            break;

        case '#': // Wall cell
            if (colA % 2 == 1 && rowA % 2 == 0) {
                // Odd column, even row: Horizontal wall
                drawFillRectangle(x, y + (cellSize / 2 - 1), cellSize, wallThickness, DARKGREY);
            } else if (colA % 2 == 0 && rowA % 2 == 1) {
                // Even column, odd row: Vertical wall
                drawFillRectangle(x + (cellSize / 2 - 1), y, wallThickness, cellSize, DARKGREY);
            } else {
                // Default fallback for unexpected wall cases
                drawFillRectangle(x, y, 4, 4, GREEN);
            }
            break;

        case 'C': // Corner cell (white 2x2)
            drawFillRectangle(x + (cellSize / 2 - 1), y + (cellSize / 2 - 1), wallThickness, wallThickness, DARKGREY);
            break;

        case 'E': // End
            drawFinishSquare(x, y, cellSize); // Draws Checkered Shape
            break;

        case 'S': // Start
            drawFillRectangle(x, y, cellSize, cellSize, RED); // Red
            break;
        case '*': //Border
            //Do nothing. Nothing ever happens.
            break;
        default: // Default case (green 4x4)
            drawFillRectangle(x, y, 4, 4, GREEN);
            break;
    }
}
