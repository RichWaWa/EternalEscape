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
