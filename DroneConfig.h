#pragma once

// --- Wi-Fi Settings ---
const char* WIFI_SSID = "Drone_ESP32";
const char* WIFI_PASS = "12345678";
const int UDP_PORT = 4210;

// --- Hardware Pins ---
const int STATUS_LED_PIN = 2;

// --- System Settings ---
const int FAILSAFE_TIMEOUT = 20000; // 2 seconds