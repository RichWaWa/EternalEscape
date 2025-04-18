#include "screen.h"
#include "settings.h" //Settings are only accessed by sub functions.

// Global variables for TFT and touchscreen
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);  // Initialize tft with required pin values
TouchScreen ts(TS_XP, TS_YP, TS_XM, TS_YM, TS_RES);  // Initialize ts with required pin values and resistance

//Button Dimentions and vars
const int16_t btnWidth = 60;  // Width of the rectangle
const int16_t btnHeight = 24; // Height of the rectangle
const int16_t xShift = 80;      // number to shift the alt text by.

bool wifiStatusLast = false;        //tracks whether the wifi status has changed or not 

//Brightness button Variables
const int16_t btnBrightnessX = 150;     // X-coordinate of the rectangle
const int16_t btnBrightnessY = 110;     // Y-coordinate of the rectangle
String brightnessLevel = "HI";          // Default brightness level
String brightnessLevelLast = "";  

//speed the players move
//time between each player moves //LO, MD, HI = 500, 300, 100
const int16_t btnPlayerSpeedX = 150 + xShift;
const int16_t btnPlayerSpeedY = 135;
String playerSpeedLevel = "HI";              // Default player speed
String playerSpeedLevelLast = "";            // Last player speed value

//speed the maze completes
//speed the maze generates at //LO, MD, HI = 80, 40, 10
const int16_t btnMazeSpeedX = 150;
const int16_t btnMazeSpeedY = 160;
String mazeSpeedLevel = "HI";                // Default maze speed
String mazeSpeedLevelLast = "";              // Last maze speed value 

//maze complete timeout (how long the finished maze stays on screen)
//time that the solved maze stays on screen //1, 2, 4 = 1000, 2000, 4000
const int16_t btnTimeoutX = 150 + xShift;
const int16_t btnTimeoutY = 185;
String victoryTimeoutLevel = "4";             // Default victory timeout
String victoryTimeoutLevelLast = "";          // Last victory timeout value

//toggles player 2
// Variable to control whether player2 is created
const int16_t btnPlayer2X = 150;
const int16_t btnPlayer2Y = 210;
String player2Level = "OFF";                    // Default Player 2 status
String player2LevelLast = "";                   // Last Player 2 status value
 
// Debounce tracking variables
unsigned long lastTouchTime = 0;
const unsigned long debounceDelay = 300; // 300ms debounce delay

// Initialize Display
void initScreen() {
    tft.begin(SPI_FREQUENCY);
    tft.setRotation(3); //sets the correct screen orientation. (SD card facing down. If you didnt do this, set it to 1)
    tft.fillScreen(ILI9341_BLACK);  
    //need to load the brightness from the settings
    loadingScreen();    //show the loading screen 
}

// Draw the settings screen initially
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
    drawSimpleButton("Brightness:", brightnessLevel, btnBrightnessX, btnBrightnessY);
    //brightnessLevelLast = brightnessLevel;
    //draw player speed toggle
    drawSimpleButton("PlayerSpeed:", playerSpeedLevel, btnPlayerSpeedX, btnPlayerSpeedY);
    playerSpeedLevelLast = playerSpeedLevel;
    //draw maze speed toggle
    drawSimpleButton("MazeSpeed:", mazeSpeedLevel, btnMazeSpeedX, btnMazeSpeedY);
    mazeSpeedLevelLast = mazeSpeedLevel;
    //draw victory timeout toggle
    drawSimpleButton("VctryTime:", victoryTimeoutLevel, btnTimeoutX, btnTimeoutY);
    victoryTimeoutLevelLast = victoryTimeoutLevel;
    //draw player2 toggle
    drawSimpleButton("Player2:", player2Level, btnPlayer2X, btnPlayer2Y);
    player2LevelLast = player2Level;
}

// Update the settings screen (partial updates only)
void updateSettingsScreen(const int16_t &x, const int16_t &y, const int16_t &z ,bool wifiStatus) {
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

    // Update settings if they have changed
    checkButtonTouch(x, y, z, btnBrightnessX, btnBrightnessY, "Brightness", brightnessLevel, toggleBrightness);
    checkButtonTouch(x, y, z, btnPlayerSpeedX, btnPlayerSpeedY, "PlayerSpeed", playerSpeedLevel, togglePlayerSpeed);
    checkButtonTouch(x, y, z, btnMazeSpeedX, btnMazeSpeedY, "MazeSpeed", mazeSpeedLevel, toggleMazeSpeed);
    checkButtonTouch(x, y, z, btnTimeoutX, btnTimeoutY, "Timeout", victoryTimeoutLevel, toggleVictoryTimeout);
    checkButtonTouch(x, y, z, btnPlayer2X, btnPlayer2Y, "Player2", player2Level, togglePlayer2);

}

// Get touch points
void getTouchPoints(int16_t& x, int16_t& y, int16_t& z) {
    TSPoint touchPoint = ts.getPoint();
    // Map x and y to origin at top left corner.
    //NOTE
    //TFT_WIDTH 320
    //TFT_HEIGHT 240
    //Note, the screens touchpoint mapping is different from how we have it installed!
    y = map(touchPoint.x, TS_MINY, TS_MAXY, 0, tft.width());
    x = map(touchPoint.y, TS_MINX, TS_MAXX, 0, tft.height()); 
    z = abs(touchPoint.z);
    //y = tft.width() - y;    // Adjust Y-axis to invert it, leaving  Y-axis: rightwards
    x = tft.height() - x;   //invert X-Axis 

    //Uncomment these lines to read the touchscreen printouts!
    if (z > MINPRESSURE && z < MAXPRESSURE) {
        Serial.print("Touch detected at: ");
        Serial.print("X=");
        Serial.print(touchPoint.x);
        Serial.print(", Y=");
        Serial.print(touchPoint.y);
        Serial.print(", Z=");
        Serial.println(touchPoint.z);

        //These lines are to confirm the mapped touch points
        Serial.print("Mapped Touch detected at: ");
        Serial.print("X=");
        Serial.print(x);
        Serial.print(", Y=");
        Serial.print(y);
        Serial.print(", Z=");
        Serial.println(z);
    }
}

//check if a button was toggled
void checkButtonTouch(const int16_t &x, const int16_t &y, const int16_t &z, int16_t btnX, int16_t btnY, const String& label, String& settingLevel, void (*onTouchCallback)()) {
    if (z > MINPRESSURE && z < MAXPRESSURE) {
        if (x >= btnX && x <= (btnX + btnWidth) &&
            y >= btnY && y <= (btnY + btnHeight)) {

            unsigned long currentTime = millis();
            if (currentTime - lastTouchTime > debounceDelay) {
                // Call the provided toggle/update function
                if (onTouchCallback != nullptr) {
                    onTouchCallback();
                }

                // Update the button appearance
                drawFillRectangle(btnX, btnY, btnWidth, btnHeight, ILI9341_WHITE);
                drawTextCentered(settingLevel.c_str(), btnX + (btnWidth / 2), btnY + (btnHeight / 2), ILI9341_BLACK);

                lastTouchTime = currentTime;
            }
        }
    }
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

void toggleBrightness() {
    // Change brightness level
    const int levelValues[3] = {85, 128, 255};
    const String levelsText[3] = {"LO", "MD", "HI"};
    Serial.println("Changing Brightness");
    toggleSettingLevel(brightnessLevel, brightnessLevelLast, brightnessPWM, levelValues, levelsText, 3);
    //if (brightnessLevel != brightnessLevelLast) {
    //    brightnessLevelLast = brightnessLevel;
    //
    //}
    Serial.println(brightnessLevel);    //Debug
    saveBrightness(brightnessPWM);    //save to settings
    analogWrite(TFT_LITE, brightnessPWM);   //set brightness level
} 

void togglePlayerSpeed() {
    const int levelValues[3] = {500, 300, 100};  // Time between player moves
    const String levelsText[3] = {"LO", "MD", "HI"};
    Serial.println("Toggling Player Speed");
    toggleSettingLevel(playerSpeedLevel, playerSpeedLevelLast, playerSpeed, levelValues, levelsText, 3);
    Serial.println(playerSpeedLevel);  // Debug
    savePlayerSpeed(playerSpeed);
}

void toggleMazeSpeed() {
    const int levelValues[3] = {80, 40, 10};  // Maze generation delay
    const String levelsText[3] = {"LO", "MD", "HI"};
    Serial.println("Toggling Maze Speed");
    toggleSettingLevel(mazeSpeedLevel, mazeSpeedLevelLast, mazeSpeed, levelValues, levelsText, 3);
    Serial.println(mazeSpeedLevel);  // Debug
    saveMazeSpeed(mazeSpeed);
}

void toggleVictoryTimeout() {
    const int levelValues[3] = {1000, 2000, 4000};  // Time maze stays on screen
    const String levelsText[3] = {"SHT", "MED", "LNG"};
    Serial.println("Toggling Victory Timeout");
    toggleSettingLevel(victoryTimeoutLevel, victoryTimeoutLevelLast, victoryTimeout, levelValues, levelsText, 3);
    Serial.println(victoryTimeoutLevel);  // Debug
    saveVictoryTimeout(victoryTimeout);
}

void togglePlayer2() {
    const int levelValues[2] = {0, 1};  // 0 = OFF, 1 = ON
    const String levelsText[2] = {"OFF", "ON"};
    int player2ToggleInt = static_cast<int>(player2Toggle);
    Serial.println("Toggling Player 2");
    toggleSettingLevel(player2Level, player2LevelLast, player2ToggleInt, levelValues, levelsText, 2);
    Serial.println(player2Level);  // Debug
    savePlayer2(player2Level);
}


// Toggle brightness levels
void toggleSettingLevel(String& currentSettingLevel, String& currentSettingLevelLast, int& currentSettingValue, const int settingLevelValues[], const String levelsText[], const int arraySize) {
    //static String defaultLevels[3] = {"LO", "MD", "HI"};
    //if (levelsText == nullptr) {
    //    levelsText = defaultLevels;
    //}
    for (size_t i = 0; i < arraySize; i++)
    {
        if (currentSettingLevel.equals(levelsText[i])) {
            i += 1;
            if (i > 2)
            {
                currentSettingLevel = levelsText[0];
                currentSettingLevel = levelsText[0];
                currentSettingValue = settingLevelValues[0];    
            }else{
                currentSettingLevel = levelsText[i];
                currentSettingLevel = levelsText[i];
                currentSettingValue = settingLevelValues[i];  
            }
            break;
        }
    }
    
}

//load and apply brightness from settings
//int loadBrightnessFromSettings(){
//    brightnessLevel = loadBrightness(); //load from settings
//    //Serial.print("Here is the brightness I loaded ");
//    //Serial.println(brightnessLevel);
//
//    if (brightnessLevel.equals("LO")) {
//        brightnessPWM = 85;     // 1/3 of 255
//    } else if (brightnessLevel.equals("MD")) {
//        brightnessPWM = 170;    // 2/3 of 255
//    } else {
//        brightnessLevel = "HI";
//        brightnessPWM = 255;    // 3/3 of 255
//        //Serial.println("Load brightness from settings defaulted");
//    }
//    return brightnessPWM;
//}
////////////////////
// Utility functions
////////////////////
void drawSimpleButton(String label, String level, int btnX, int btnY) {
    //Caculate offsets (for shifting text over)
    const int labelYOffset = +3;

    drawText(label.c_str(), btnX-140, btnY + labelYOffset, ILI9341_WHITE);               // Draw label
    drawFillRectangle(btnX, btnY, btnWidth, btnHeight, ILI9341_WHITE);                 // Draw button rectangle
    drawTextCentered(level.c_str(), btnX + (btnWidth / 2), btnY + (btnHeight / 2), ILI9341_BLACK); // Centered text
}

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
        case '.': // Path cell (black 14x14) //'.' is uncomputed path, '0' is computed path
            drawFillRectangle(x, y, cellSize, cellSize, BLACK);
            break;

        case '0': // Path cell (black 14x14) //'.' is uncomputed path, '0' is computed path
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

        case 'F': // Frontier
            drawFillRectangle(x, y, cellSize, cellSize, LIGHTRED);
            break;
        
        case 'O': // Orange Player
            drawFillRectangle(x, y, cellSize, cellSize, ORANGE);
            break;

        case 'B': // Blue Player
            drawFillRectangle(x, y, cellSize, cellSize, BLUE);
            break;

        case 'X': // Orange Path
            if (colA % 2 == 1 && rowA % 2 == 0) {
                // Odd column, even row: Horizontal wall
                drawFillRectangle(x, y + (cellSize / 2 - 1), cellSize, wallThickness, LIGHTORANGE);
            } else if (colA % 2 == 0 && rowA % 2 == 1) {
                // Even column, odd row: Vertical wall
                drawFillRectangle(x + (cellSize / 2 - 1), y, wallThickness, cellSize, LIGHTORANGE);
            } else {
                drawFillRectangle(x, y, cellSize, cellSize, LIGHTORANGE);
            }
            break;

        case 'x': // Orange Backtrack Path
            if (colA % 2 == 1 && rowA % 2 == 0) {
                // Odd column, even row: Horizontal wall
                drawFillRectangle(x, y + (cellSize / 2 - 1), cellSize, wallThickness, DARKORANGE);
            } else if (colA % 2 == 0 && rowA % 2 == 1) {
                // Even column, odd row: Vertical wall
                drawFillRectangle(x + (cellSize / 2 - 1), y, wallThickness, cellSize, DARKORANGE);
            } else {
                drawFillRectangle(x, y, cellSize, cellSize, DARKORANGE);
            }
            break;

        case 'Z': // Blue Path
            if (colA % 2 == 1 && rowA % 2 == 0) {
                // Odd column, even row: Horizontal wall
                drawFillRectangle(x, y + (cellSize / 2 - 1), cellSize, wallThickness, LIGHTBLUE);
            } else if (colA % 2 == 0 && rowA % 2 == 1) {
                // Even column, odd row: Vertical wall
                drawFillRectangle(x + (cellSize / 2 - 1), y, wallThickness, cellSize, LIGHTBLUE);
            } else {
                drawFillRectangle(x, y, cellSize, cellSize, LIGHTBLUE);
            }
            break;

        case 'z': // Blue Backtrack Path
            if (colA % 2 == 1 && rowA % 2 == 0) {
                // Odd column, even row: Horizontal wall
                drawFillRectangle(x, y + (cellSize / 2 - 1), cellSize, wallThickness, DARKBLUE);
            } else if (colA % 2 == 0 && rowA % 2 == 1) {
                // Even column, odd row: Vertical wall
                drawFillRectangle(x + (cellSize / 2 - 1), y, wallThickness, cellSize, DARKBLUE);
            } else {
                drawFillRectangle(x, y, cellSize, cellSize, DARKBLUE);
            }
            break;  

        case '*': //Border
            //Do nothing. Nothing ever happens.
            break;
        default: // Default case (green 4x4)
            drawFillRectangle(x, y, 4, 4, GREEN);
            break;
    }
}
