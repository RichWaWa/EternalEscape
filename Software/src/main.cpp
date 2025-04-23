#include <Adafruit_ILI9341.h> // Include the Adafruit ILI9341 library
#include "TouchScreen.h" // Include the Adafruit Touchscreen library
#include "screen.h"     //screen class
#include "networking.h" //Networking class
#include "settings.h"   //store device settings
#include "mazeGen.h"
#include "mazeSolver.h"

// Pin definitions
#define NEOPIXEL_PIN 33        // Pin for the onboard NeoPixel
#define NUMPIXELS 1            // Number of NeoPixels on the board

//Touch screen variables
int16_t x, y, z;

//Misc Global variables
unsigned long lastPlayerMoveTime = 0;
unsigned long mazeSolvedTime = 0;
bool mazeSolved = false;
vector<pair<int, int>> tempPosition = {{0, 0}};
vector<vector<char>> builtMaze;
bool playerSetupComplete = false;
static const int holdToggleValue = 3000;        //value for how long you need to hold on the display to access the settings.
static const int debounceDelay = 500;           //delay for the touch debounce.

//Misc Settings TODO: Move these to Settings
//unsigned const int playerMoveSpeed = 100;       //time between each player moves //LO, MD, HI = 500, 300, 100
//const int mazeGenerateSpeed = 10;               //speed the maze generates at //LO, MD, HI = 80, 40, 10
//unsigned int mazeSolvedScreenTimeout = 2000;    //time that the solved maze stays on screen //1, 2, 4 = 1000, 2000, 4000
//bool enablePlayer2 = true;                      // Variable to control whether player2 is created

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
  pinMode(TFT_LITE, OUTPUT); //set the PWM output for the backlight
  initializePreferences();
  //load preferences.
  //waitForSerial();  //debug statement 
  brightnessPWM = loadBrightness();
  analogWrite(TFT_LITE, brightnessPWM);   //set brightness level
  playerSpeed = loadPlayerSpeed();
  mazeSpeed = loadMazeSpeed();
  victoryTimeout = loadVictoryTimeout();
  player2Toggle = loadPlayer2();
  initSettingsTextLevels(); //load the settings levels for the buttons

  // Initialize the TFT display
  initScreen();
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
  //create player1 as color Orange
  static Player player1('O');
  // Conditionally create player2 as color Blue
  static Player* player2 = nullptr;


  //Start main programloop for the maze screen state
  if (currentState == MAZESCREEN) {
    if(!mazeScreenOpenLast || (mazeSolved && (millis() - mazeSolvedTime > victoryTimeout))){
      //draw the maze initially
      generateMaze(mazeSpeed);
      builtMaze = mazeGetter();
      mazeSolved = false;
      playerSetupComplete  = false;
      //run last
      mazeScreenOpenLast = true;  //update to true to indicate its been run initially
    }else{
      //Run all the player movement stuff
      tempPosition = {{0, 0}};
      if(!mazeSolved && !playerSetupComplete){
        // Find Start and store in player positions
        player1.clearPositions();  //reset the path
        player1.addPosition(getStartPositions()[0].first, getStartPositions()[0].second);
        //Determine if we need to enable player two, if we do, emable and set it up.
        if (player2Toggle && !player2) {
          Serial.println("Creating player2");
          player2 = new Player('B');
        } else if (!player2Toggle && player2) {
          // Delete player2 if toggle is disabled and player2 exists
          Serial.println("Deleting player2");
          player2->clearPositions(); // Clear player2's positions
          delete player2;
          player2 = nullptr;
        }

        if (player2) {
          player2->clearPositions();
          player2->addPosition(getStartPositions()[1].first, getStartPositions()[1].second);
        }
        playerSetupComplete = true;
        Serial.println("Player Setup Complete");
      }
      //update the maze as the player moves here.
      if ((millis() - lastPlayerMoveTime > playerSpeed) && !mazeSolved) {
        lastPlayerMoveTime = millis();
        // Player code calls go here
        // Update the solver path with one recursive step.
        player1.calculateMove(builtMaze);
        // Player 2 moves if it exists
        if (player2) {
          player2->calculateMove(builtMaze);
        }
        // Check if player1 or player2 has solved the maze (if the last cell is 'E')
        if ((!player1.getPositions().empty() && builtMaze[player1.getPositions().back().first][player1.getPositions().back().second] == 'E') ||
            (player2 && !player2->getPositions().empty() && builtMaze[player2->getPositions().back().first][player2->getPositions().back().second] == 'E')) {
          mazeSolvedTime = millis();
          Serial.println("Maze Solved!");
          mazeSolved = true;
        }
        return;
      }
    }//end mazeScreenOpenLast else
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
