# EternalEscape
## Main Project Repo for the EternalEscape desktop toy!
**EternalEscape** is a desktop toy that generates and solves random mazes for endless entertainment and distraction from any productivity! 
Designed to be both entertaining and mesmerizing, it’s the perfect addition to your workspace. 
The project is fully open-source and can be built using common materials and components.

---

## Features

- **Random Maze Generation:** Creates unique mazes endlessly.  
- **Real-Time Maze Solving:** Watch as the algorithm finds its way out, again and again.  
- **Compact Design:** Fully 3D printable with common hardware components.  
- **Expandable Functionality:** Networking and a settings GUI are planned for future updates.  

---

## Hardware Requirements

- **3D Printed Parts:** Designed for an Ender3 printer. CAD files are in Inventor format, with STLs provided.  
- **Fasteners:** (items marked with '[!]' are optional, and could be replaced with hot glue.)
  - 2 × M3x5mm bolts  [!]
  - 4 × M3x8mm bolts  
  - 4 × M2x3mm bolts  [!]
- **Electronics:**
  - Adafruit Feather ESP32-S3 (2MB PSRAM)  
  - Adafruit 2.8" ILI9341 TFT Resistive Touch Display  

---

## Software

All source code is written in **C++** for efficient and robust performance. Future updates will include networking capabilities and a GUI for adjusting device settings.

---

## Assembly

1. **Print the Parts:** Use the provided STL files to 3D print all necessary components.  
2. **Hardware Assembly:** Secure the parts using the listed bolts.  
3. **Connect Electronics:** Follow the wiring diagram (to be added) to connect the display to the ESP32-S3.  
4. **Upload the Code:** Flash the provided C++ code to the ESP32-S3 using your preferred IDE.  

---

## Future Roadmap

- **Networking:** Enable cloud-based maze sharing or remote control.  
- **Customizable Settings:** Implement a GUI for adjusting speed, maze size, and algorithm behavior.  

---

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests to help make EternalEscape even better.

---

## License

This project is licensed under the MIT License. See `LICENSE` for details.
