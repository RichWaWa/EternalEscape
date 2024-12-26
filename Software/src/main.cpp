#include <Adafruit_ILI9341.h> // Include the Adafruit ILI9341 library
#include <Adafruit_NeoPixel.h> // Include the NeoPixel library
#include "TouchScreen.h" // Include the Adafruit Touchscreen library
#include "screen.h"

// Pin definitions
#define NEOPIXEL_PIN 33        // Pin for the onboard NeoPixel
#define NUMPIXELS 1            // Number of NeoPixels on the board

// Screen instance and variables
int16_t x, y, z;
ScreenManager screen = ScreenManager();

// Create instances for the display and NeoPixel
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Global variables for color cycling
int colorIndex = 0;
uint16_t colors[] = {ILI9341_RED, ILI9341_GREEN, ILI9341_BLUE};

//Program setup
void setup() {
  Serial.begin(115200);
  analogReadResolution(10);
  Serial.println("Initializing Display, NeoPixel, and Touchscreen");

  // Initialize the TFT display
  screen.init();

  // Initialize the NeoPixel
  pixels.begin();
  pixels.show(); // Turn off all pixels at startup
}

void loop() {
  //get the touchpoints from the display
  screen.getTouchPoints(x,y,z);
  
  // If the screen is touched
  if (z > MINPRESSURE && z < MAXPRESSURE) {
    // Cycle to the next color
    colorIndex = (colorIndex + 1) % 3; // Cycle through 0, 1, 2
    uint16_t currentColor = colors[colorIndex];

    // Update the display and NeoPixel
    screen.drawFillScreen(currentColor);
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