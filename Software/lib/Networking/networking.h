#ifndef NETWORKING_H
#define NETWORKING_H

#include <WiFi.h> // Include WiFi library for network functionality

// Function to get the MAC address
String getMACAddress();
// Function to check Wi-Fi connection status
bool isWiFiConnected();

#endif // NETWORKING_H
