#include <Adafruit_ILI9341.h> // Include the Adafruit ILI9341 library
#include <Adafruit_NeoPixel.h> // Include the NeoPixel library
#include "TouchScreen.h" // Include the Adafruit Touchscreen library
#include "screen.h"     //screen class
#include "networking.h" //Networking class
#include "settings.h"   //store device settings
#include "mazeGen.h"
#include "player.h"
#include "mazeSolver.h"

// Pin definitions
#define NEOPIXEL_PIN 33        // Pin for the onboard NeoPixel
#define NUMPIXELS 1            // Number of NeoPixels on the board

//Touch screen variables
int16_t x, y, z;

//Misc Global variables
unsigned long lastPlayerMoveTime = 0;
bool mazeSolved = false;
vector<pair<int, int>> tempPosition = {{0, 0}};
vector<vector<char>> builtMaze;
//Misc Settings TODO: Move these
unsigned const long playerMoveSpeed = 500;
static const int holdToggleValue = 3000;        //value for how long you need to hold on the display to access the settings.
static const int debounceDelay = 500;           //delay for the touch debounce.

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
//generic function declerations
void waitForSerial(unsigned long timeout = 10000);  //default value of 10000
void settings();  //Settings
void maze();  //Maze

//Mac address
String macAddr;

//Variables for tracking the first function call for the settings or the Maze Screen
bool settingsOpenLast = false;  //is the settings screen previously open?
bool mazeScreenOpenLast = false; //is the maze screen previously open?

//Program setup
void setup() {
  Serial.begin(115200);
  analogReadResolution(10);
  srand(time(0));  // Seed random number generator
  // Initialize the TFT display
  initScreen();
  waitForSerial();  //debug statement 

  //Get MAC address
  macAddr = getMACAddress();
  //Connect to WiFi 
  connectToWiFi();
  unsigned long mainStartTime = millis();
}

////////////////////////////////////////////////////
//Main Program Loop
////////////////////////////////////////////////////
void loop() {
  //get the touchpoints from the display
  getTouchPoints(x,y,z); 
  updateStateMachine();
  
  //Settings
  settings();

  //Maze
  maze();
}//End Main loop

////////////////////////////////////////////////////
//Function Declarations
////////////////////////////////////////////////////

//State Machine updates for display
void updateStateMachine() {
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
}//END State machine block

//Draw settings screen
void settings(){
  if (currentState == SETTINGS) {
    if(!settingsOpenLast){
      //draw everything the first go around
      drawSettingsScreen(macAddr, isWiFiConnected());
      settingsOpenLast = true;  //update to true to indicate its been run initially
    }else{
      //otherwise, we know everything else is drawn so we can just update it.
      updateSettingsScreen(x, y, z, isWiFiConnected());
    }
    
  }else{
    settingsOpenLast = false; //settings tab closed
  }
}//end settings

//Draw the maze
void maze(){
  //create player
  static Player player;
  if (currentState == MAZESCREEN) {
    if(!mazeScreenOpenLast){
      //draw the maze initially
      drawFillScreen(0xFFFF); //Fill screen with white
      generateMaze();
      builtMaze = mazeGetter();
      mazeSolved = false;
      //run last
      mazeScreenOpenLast = true;  //update to true to indicate its been run initially
    }else{
      tempPosition = {{0, 0}};
      //Player player;
      if(!mazeSolved){
        // Find Start and store in player positions
        player.clearPositions(); // Ensure it's empty before starting
        player.addPosition(getStartPositions()[0].first, getStartPositions()[0].second);
      }

      //update the maze as the player moves here.
      if (millis() - lastPlayerMoveTime > playerMoveSpeed) {
        lastPlayerMoveTime = millis();
        // Player code calls go here
        // Update the solver path with one recursive step.
        randRecursiveMazeSolver(builtMaze, player.getPositions(), 'O', 'x');
        // Check if we've reached the end (for example, if the last cell is 'E')
        if (!player.getPositions().empty() && mazeGetter()[player.getPositions().back().first][player.getPositions().back().second] == 'E') {
            mazeSolved = true;
            Serial.println("Maze Solved!");
        }
        Serial.println("The player shall move now!");
        return;
      }

    }//end open last else
  }else{
    mazeScreenOpenLast = false; //maze screen closed
  }//END Display Screen updates
}

//Debug statement
void waitForSerial(unsigned long timeout) { //Waits for a serial connection before pursuing with the program (FOR DEBUG USE ONLY)
  unsigned long startTime = millis();
  // Wait for Serial to connect
  Serial.println("Waiting for serial connection...");
  while (!Serial) {
      if (millis() - startTime > timeout) {
          Serial.println("Serial connection timeout. Continuing without serial...");
          return;
      }
  }
  Serial.println("Serial connection established.");
}//END waitForSerial
