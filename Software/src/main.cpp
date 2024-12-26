#include <Adafruit_ILI9341.h> // Include the Adafruit ILI9341 library
#include <Adafruit_NeoPixel.h> // Include the NeoPixel library
#include "TouchScreen.h" // Include the Adafruit Touchscreen library

// Pin definitions
#define NEOPIXEL_PIN 33        // Pin for the onboard NeoPixel
#define NUMPIXELS 1            // Number of NeoPixels on the board

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

// Touchscreen calibration values (replace with your display's calibration values)
#define TS_MINX 150            // 150
#define TS_MINY 120            // 120
#define TS_MAXX 920            // 920
#define TS_MAXY 940            // 940
// define valid pressure ranged to eliminate error
#define MINPRESSURE 250
#define MAXPRESSURE 10000

// Touchscreen instance and variables
TouchScreen ts = TouchScreen(TS_XP, TS_YP, TS_XM, TS_YM, 332);
int16_t x;
int16_t y;
int16_t z;

// Create instances for the display and NeoPixel
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Global variables for color cycling
int colorIndex = 0;
uint16_t colors[] = {ILI9341_RED, ILI9341_GREEN, ILI9341_BLUE};

//Function Declarations
void getTouchPoints(int16_t &a,int16_t &b,int16_t &c);    //gets the touchpoints for the display.


//Program setup
void setup() {
  Serial.begin(115200);
  analogReadResolution(10);
  Serial.println("Initializing Display, NeoPixel, and Touchscreen");

  // Initialize the TFT display
  tft.begin(SPI_FREQUENCY);
  tft.setRotation(1); // Set rotation (optional)

  // Initialize the NeoPixel
  pixels.begin();
  pixels.show(); // Turn off all pixels at startup

  // Clear the screen
  tft.fillScreen(ILI9341_BLACK);
}

void loop() {
  //get the touchpoints from the display
  getTouchPoints(x,y,z);
  

  // If the screen is touched
  if (z > MINPRESSURE && z < MAXPRESSURE) {
    // Cycle to the next color
    colorIndex = (colorIndex + 1) % 3; // Cycle through 0, 1, 2
    uint16_t currentColor = colors[colorIndex];

    // Update the display and NeoPixel
    tft.fillScreen(currentColor);
    switch (colorIndex) {
      case 0: pixels.setPixelColor(0, pixels.Color(255, 0, 0)); break; // Red
      case 1: pixels.setPixelColor(0, pixels.Color(0, 255, 0)); break; // Green
      case 2: pixels.setPixelColor(0, pixels.Color(0, 0, 255)); break; // Blue
    }
    pixels.show();
    // Debounce delay
    delay(300);
  }

}

void getTouchPoints(int16_t &a,int16_t &b,int16_t &c){
  // Get and map touch coordinates
  TSPoint touchPoint = ts.getPoint();
  a = map(touchPoint.x, TS_MINX, TS_MAXX, 0, tft.width());
  b = map(touchPoint.y, TS_MINY, TS_MAXY, 0, tft.height());
  c = abs(touchPoint.z);
  //Serial.print("Touch detected at: ");
  //Serial.print("X=");
  //Serial.print(x);
  //Serial.print(", Y=");
  //Serial.print(y);
  //Serial.print(", Z=");
  //Serial.println(z);

}
