#include "Networking.h"

// Function to retrieve the MAC address
String getMACAddress() {
    return WiFi.macAddress(); // Returns the MAC address as a string
}

// Function to check Wi-Fi connection status
bool isWiFiConnected() {
     // Returns true if connected, false otherwise
    if(WiFi.status() == WL_CONNECTED){
        return true;
    }else{
        return false;
    }
}

// Function to connect to available networks
void connectToWiFi() {
    // Initialize WiFi in station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // Clear previous connections
    delay(100);

    // Scan for available networks with a timeout
    Serial.println("Scanning WiFi networks...");
    unsigned long scanStart = millis();
    int numNetworks;
    while ((numNetworks = WiFi.scanNetworks()) == -1) {
        if (millis() - scanStart > 10000) { // 10-second timeout
            Serial.println("WiFi scan timeout. No networks found.");
            return;
        }
        delay(100);
    }

    if (numNetworks == 0) {
        Serial.println("No WiFi networks found.");
        return;
    }

    Serial.printf("%d networks found.\n", numNetworks);
    for (int i = 0; i < numNetworks; ++i) {
        // Display network info (SSID and signal strength)
        Serial.printf("Network %d: %s (Signal: %d dBm, Encryption: %s)\n", 
                      i + 1, 
                      WiFi.SSID(i).c_str(), 
                      WiFi.RSSI(i),
                      WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Secured");

        // Attempt to connect to open networks (no password required)
        if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
            Serial.printf("Attempting to connect to open network: %s\n", WiFi.SSID(i).c_str());
            WiFi.begin(WiFi.SSID(i).c_str());
            unsigned long connectStart = millis();
            while (WiFi.status() != WL_CONNECTED) {
                if (millis() - connectStart > 5000) { // 5-second timeout for connection attempt
                    Serial.println("Connection attempt timed out. Trying next network...");
                    break;
                }
                delay(100);
            }
            if (WiFi.status() == WL_CONNECTED) {
                Serial.printf("Connected successfully to: %s\n", WiFi.SSID(i).c_str());
                return;
            }
        }
    }
    // If no connection was successful
    Serial.println("Failed to connect to any network.");
}