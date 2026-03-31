#pragma once
#include <WiFi.h>
#include <WiFiUdp.h>
#include "DroneConfig.h"

// A clean struct to hold our flight data
struct FlightCommand {
  float pitch = 0;
  float roll = 0;
  float yaw = 0;
  float throttle = 0;
  bool updatedThisLoop = false;
};

class DroneNetwork {
  private:
    WiFiUDP udp;
    IPAddress remoteIP;
    int remotePort;
    bool connected;
    unsigned long lastCommandTime;

  public:
    DroneNetwork() : remotePort(0), connected(false), lastCommandTime(0) {}

    void begin() {
      WiFi.softAP(WIFI_SSID, WIFI_PASS);
      udp.begin(UDP_PORT);
      Serial.print("Drone AP IP: ");
      Serial.println(WiFi.softAPIP());
    }

    bool isConnected() { return connected; }
    unsigned long getLastCommandTime() { return lastCommandTime; }
    void forceDisconnect() { connected = false; }

    void processIncoming(FlightCommand& cmd, unsigned long currentMillis) {
      cmd.updatedThisLoop = false;
      int packetSize = udp.parsePacket();
      
      if (!packetSize) return;

      lastCommandTime = currentMillis;
      char packet[255];
      int len = udp.read(packet, 255);
      if (len > 0) packet[len] = '\0';

      remoteIP = udp.remoteIP();
      remotePort = udp.remotePort();

      if (strncmp(packet, "CONNECT", 7) == 0) {
        udp.beginPacket(remoteIP, remotePort);
        udp.print("CONNECTED");
        udp.endPacket();
        connected = true;
        Serial.println("--> Phone Connected!");
      } 
      else if (strncmp(packet, "ACTION:", 7) == 0) {
        Serial.printf("--> Action: %s\n", packet);
      } 
      else {
        if (sscanf(packet, "%f,%f,%f,%f", &cmd.pitch, &cmd.roll, &cmd.yaw, &cmd.throttle) == 4) {
          cmd.updatedThisLoop = true;
        }
      }
    }

    void sendTelemetry(unsigned long currentMillis) {
      static unsigned long lastTelemetry = 0;
      if (connected && currentMillis - lastTelemetry > 1000) {
        lastTelemetry = currentMillis;
        udp.beginPacket(remoteIP, remotePort);
        udp.printf("WiFi telemetry: Battery 84%% ALT 12.5m");
        udp.endPacket();
      }
    }
};