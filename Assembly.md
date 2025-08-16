# EternalEscape Assembly guide
 - REV 0
 - RELEASE 1.0
---

This guide will walk you through the steps of printing, calibrating, programming, and assembling your own EternalEscape toy!
Please read through this document carefully before starting the assembly.
---

### **1. Bill of Materials**

Below is a list of hardware and tools needed to build EternalEscape. 
Some parts are optional and can be substituted for hot glue or double-sided tape. 

  * **3D Printed Parts:** STL files located in EternalEscape/Hardware/Exports/
  * **Electronics:**
    - Adafruit Feather ESP32-S3 (2MB PSRAM)
    - Adafruit 2.8" ILI9341 TFT Resistive Touch Display
    - Adafruit Female-to-female jumper wires or 22AWG solid core wire
  * **Fasteners:**
    - 4 × M3x8mm Socket head bolts
    - 2 × M3x5mm Socket head bolts (optional)
    - 4 × M2x3mm Socket head bolts (optional)
  * **Software:**
    - Microsoft Visual Studio Code (VSCode)
    - PlatformIO IDE extension for VSCode

---

### **2. Assembling the Electronics**

The wiring for the Feather ESP32-S3 and the TFT display is critical. It's recommended to use a breadboard for initial testing before making permanent connections.

#### **Wiring Diagram**

Connect the display's pins to the ESP32-S3 as follows:

  * **Display GND** to **ESP32 GND**
  * **Display VCC** to **ESP32 3.3V**
  * **Display CLK** to **ESP32 SCK** (hardware SPI SCK pin)
  * **Display MOSI** to **ESP32 MO** (hardware SPI MOSI pin)
  * **Display MISO** to **ESP32 MI** (hardware SPI MISO pin)
  * **Display CS** to **ESP32 GPIO9** 
  * **Display D/C** to **ESP32 GPIO6**
  * **Display RST** to **ESP32 GPIO5**
  * **Display Lite** to **ESP32 GPIO10**
  * **Touch Y+** to **ESP32 A4 (GPIO14)**
  * **Touch X+** to **ESP32 A3 GPIO15**
  * **Touch Y-** to **ESP32 A2 GPIO16**
  * **Touch X-** to **ESP32 A1 GPIO17**


> **Note:** A wiring diagram can be found in EternalEscale/Hardware/EternalEscape_Wiring_Diagram.jpg

---

### **3. Programming the Toy**

This process involves three key steps: flashing the calibration code, getting the calibration values, and then uploading the main program.

#### **Step 3.1: Setting Up the Project in VSCode**

1.  **Clone or Download:** Begin by cloning or downloading the entire EternalEscape repository from GitHub.
2.  **Install PlatformIO:** Follow the instructions in [this video](https://www.google.com/search?q=https://www.youtube.com/watch%3Fv%3DR9j-o3c20Kk) to install the PlatformIO IDE extension for VSCode.
3.  **Open the Project:** Once the extension is installed, open the `Software` folder of the repository in VSCode. PlatformIO will automatically recognize the project.

#### **Step 3.2: Uploading the Calibration Code**

The touch display needs to be calibrated to accurately read touch input.

1.  **Open Calibration Sketch:** Locate the calibration sketch in your `Software` folder.
2.  **Upload:** Follow the instructions in the video to upload the calibration code to your ESP32-S3. This will compile and flash the code to the board.
3.  **Calibrate:** Follow the on-screen instructions on the toy's display to calibrate the touch points. This involves touching specific locations as prompted.

#### **Step 3.3: Grabbing the Calibration Values**

After calibration, the program will print the calibration values to the serial monitor.

1.  **Open Serial Monitor:** In the PlatformIO toolbar in VSCode, open the Serial Monitor.
2.  **Record Values:** The calibration values will be displayed in the terminal window. Copy these numbers. They will be similar to `ts.calibrate( ... );`.

#### **Step 3.4: Modifying and Uploading the Main Code**

Now, you will take those calibration values and use them in the main program.

1.  **Edit the Main Code:** Open the main `EternalEscape.cpp` file. Find the line where the `ts.calibrate()` function is called.
2.  **Paste Values:** Replace the placeholder values with the calibration numbers you copied from the serial monitor.
3.  **Upload the Final Program:** Use the PlatformIO upload function again to compile and upload the main program with your new calibration values to the EternalEscape toy.

-----

### **4. Final Assembly**

With the code uploaded, you can now fasten the toy's physical components together.

1.  **Mount the Electronics:** Carefully place the ESP32-S3 and the TFT display into the 3D printed case.
2.  **Secure the Parts:** Use the M3x8mm bolts to secure the case components. If you're using them, use the optional M3x5mm and M2x3mm bolts for any internal mounting points, as detailed in the CAD files. You can use hot glue as an alternative for the optional fasteners.
3.  **Check Connections:** Double-check all the wiring one last time to ensure everything is securely connected and not pinched.
4.  **Power On:** Connect the device to a USB power source, and your EternalEscape toy should power up, generate a maze, and begin solving it\!
