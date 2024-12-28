#include <Adafruit_ILI9341.h> // Include the Adafruit ILI9341 library
#include <Adafruit_NeoPixel.h> // Include the NeoPixel library
#include "TouchScreen.h" // Include the Adafruit Touchscreen library
#include "screen.h"     //screen class
#include "networking.h" //Networking class
#include "settings.h"   //store device settings

// Pin definitions
#define NEOPIXEL_PIN 33        // Pin for the onboard NeoPixel
#define NUMPIXELS 1            // Number of NeoPixels on the board

//Touch screen variables
int16_t x, y, z;

// Create instances for the display and NeoPixel
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Global variables for color cycling
int colorIndex = 0;
uint16_t colors[] = {ILI9341_RED, ILI9341_GREEN, ILI9341_BLUE};

//State Machine for Display
enum State { MAZESCREEN, SETTINGS };
State currentState = MAZESCREEN;
unsigned long touchStartTime = 0;
bool touchHeld = false;
//state machine debounce variables
static unsigned long lastTouchEndTime = 0; // Tracks when the touch was last released
static unsigned long lastValidTouchTime = 0; //Tracks the last time a valid touch was registered.
static bool debounceInProgress = false;   // Tracks if debounce is active
void updateStateMachine();  //update state machine function

//Mac address
String macAddr;

//Variables for tracking the first function call for the settings or the Maze Screen
bool settingsOpenLast = false;  //is the settings screen previously open?
bool mazeScreenOpenLast = false; //is the maze screen previously open?

//Program setup
void setup() {
  Serial.begin(115200);
  analogReadResolution(10);
  Serial.println("Initializing");

  // Initialize the TFT display
  initScreen();

  // Initialize the NeoPixel
  pixels.begin();
  pixels.show(); // Turn off all pixels at startup

  //Get MAC address
  macAddr = getMACAddress();
}

////////////////////////////////////////////////////
//Main Program Loop
////////////////////////////////////////////////////
void loop() {
  static unsigned long strobeTimer = 0;
  //get the touchpoints from the display
  getTouchPoints(x,y,z);
  updateStateMachine();

  ////////////////////////////////////////////////////
  //Update display Into Maze or Settings mode
  ////////////////////////////////////////////////////

  //Settings
  if (currentState == SETTINGS) {
    if(!settingsOpenLast){
      //draw everything the first go around
      drawSettingsScreen(macAddr, isWiFiConnected);
      settingsOpenLast = true;  //update to true to indicate its been run initially
    }else{
      //otherwise, we know everything else is drawn so we can just update it.
      updateSettingsScreen(isWiFiConnected);
    }
    
  }else{
    settingsOpenLast = false; //settings tab closed
  }
  //Maze
  if (currentState == MAZESCREEN) {
    if(!mazeScreenOpenLast){
      //draw the maze initially


      //run last
      mazeScreenOpenLast = true;  //update to true to indicate its been run initially
    }else{
      //update the maze as the player moves here.


      if (millis() - strobeTimer >= 1000) {
      strobeTimer = millis();

      static int colorIndex = 0;
      uint16_t colors[] = {ILI9341_RED, ILI9341_GREEN, ILI9341_BLUE};
      drawStrobeScreen(colors[colorIndex]);

      switch (colorIndex) {
        case 0: pixels.setPixelColor(0, pixels.Color(255, 0, 0)); break;
        case 1: pixels.setPixelColor(0, pixels.Color(0, 255, 0)); break;
        case 2: pixels.setPixelColor(0, pixels.Color(0, 0, 255)); break;
      }
      pixels.show();
      colorIndex = (colorIndex + 1) % 3;
      }
    }//end open last else
  }else{
    mazeScreenOpenLast = false; //maze screen closed
  }//END Display Screen updates
}//End Main loop


////////////////////////////////////////////////////
//Function Declarations
////////////////////////////////////////////////////

//State Machine updates for display
void updateStateMachine() {
  static int holdToggleValue = 3000;        //value for how long you need to hold on the display to access the settings.
  static int debounceDelay = 500;           //delay for the touch debounce.


  // Check if the touch is within the valid range
  if (z > MINPRESSURE && z < MAXPRESSURE) {
    lastValidTouchTime = millis(); //update the last valid touch time
    if (!touchHeld) {
      // Start timing when touch is first detected
      touchStartTime = millis();
      touchHeld = true;
    } else if (!debounceInProgress && millis() - touchStartTime >= holdToggleValue) {
      // User has consistently held touch for 3 seconds, without debounce
      debounceInProgress = true; // Begin debounce to prevent multiple triggers
      touchHeld = false;         // Reset touchHeld to prepare for next touch
      lastTouchEndTime = millis();

      // Toggle the state machine
      if (currentState == MAZESCREEN) {
        currentState = SETTINGS;
      } else {
        currentState = MAZESCREEN;
      }
    }
  } else {
    // No touch detected 
    //Has there been no touch detected for longer than the debounce delay?
    if((millis() - lastValidTouchTime) >= debounceDelay){
      if (touchHeld) {
        // Reset if touch is released before the hold duration is reached
        touchHeld = false;
        touchStartTime = 0;
      }
      // Ensure a debounce delay before allowing another touch event
      if (debounceInProgress && millis() - lastTouchEndTime >= debounceDelay) { // debounce
        debounceInProgress = false;
      }
    }
  }
Serial.println(touchHeld);  //DEBUG STATEMENT
}//END State machine block
