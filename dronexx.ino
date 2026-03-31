#include "DroneConfig.h"
#include "DroneStatusLED.h"
#include "DroneNetwork.h"

// Instantiate our modules
DroneStatusLED statusLed(STATUS_LED_PIN);
DroneNetwork network;
FlightCommand currentFlightData;

void setup() {
  Serial.begin(115200);
  delay(1000);

  statusLed.begin();
  network.begin();
  
  Serial.println("Drone OS Booted and Ready.");
}

void loop() {
  unsigned long currentMillis = millis();

  // --- 1. FAILSAFE ---
  if (network.isConnected() && (currentMillis - network.getLastCommandTime() > FAILSAFE_TIMEOUT)) {
    Serial.println("\n[!] WATCHDOG: Signal lost! Disconnecting.");
    network.forceDisconnect();
    // TODO: killMotors();
  }

  // --- 2. NETWORK IN ---
  network.processIncoming(currentFlightData, currentMillis);

  // --- 3. FLIGHT LOGIC ---
  if (currentFlightData.updatedThisLoop) {
    // We only print occasionally to avoid flooding the serial monitor
    static unsigned long lastPrint = 0;
    if (currentMillis - lastPrint > 250) {
      Serial.printf("P:%.2f R:%.2f Y:%.2f T:%.2f\n", 
        currentFlightData.pitch, currentFlightData.roll, 
        currentFlightData.yaw, currentFlightData.throttle);
      lastPrint = currentMillis;
    }
    
    // TODO: Send currentFlightData to MotorMixer class
  }

  // --- 4. NETWORK OUT ---
  network.sendTelemetry(currentMillis);

  // --- 5. HARDWARE UPDATE ---
  statusLed.update(network.isConnected(), currentMillis);
}