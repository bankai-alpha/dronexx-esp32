#pragma once
#include <Arduino.h>

class DroneStatusLED {
  private:
    int pin;
    unsigned long lastUpdate;
    int brightness;
    int fadeAmount;

  public:
    DroneStatusLED(int ledPin) : pin(ledPin), lastUpdate(0), brightness(0), fadeAmount(5) {}

    void begin() {
      ledcAttach(pin, 5000, 8); 
      ledcWrite(pin, 0);
    }

    void update(bool isConnected, unsigned long currentMillis) {
      int pulseInterval = isConnected ? 20 : 5;   
      int pulseStep = isConnected ? 1 : 15;       

      if (currentMillis - lastUpdate > pulseInterval) {
        lastUpdate = currentMillis;

        fadeAmount = (fadeAmount > 0) ? pulseStep : -pulseStep;
        brightness += fadeAmount;

        if (brightness >= 255) {
          brightness = 255;
          fadeAmount = -pulseStep;
        } else if (brightness <= 0) {
          brightness = 0;
          fadeAmount = pulseStep; 
        }

        ledcWrite(pin, brightness);
      }
    }
};