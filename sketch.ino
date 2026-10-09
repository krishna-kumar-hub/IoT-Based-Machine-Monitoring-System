
#include <Arduino.h>
#include <math.h>

// Built-in LED on the typical ESP32 DevKit
const int LED_PIN = 2;

// Monitoring thresholds
const float TEMP_LIMIT = 60.0;
const float VIBRATION_LIMIT = 2.5;

unsigned long lastUpdate = 0;
const unsigned long SAMPLE_INTERVAL = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.println();
  Serial.println("================================");
  Serial.println(" IoT MACHINE MONITORING SYSTEM");
  Serial.println(" Mode: SENSOR SIMULATION");
  Serial.println("================================");
}

void loop() {
  unsigned long now = millis();

  // Take a simulated sample every 2 seconds
  if (now - lastUpdate >= SAMPLE_INTERVAL) {
    lastUpdate = now;

    // Repeat the demo cycle every 60 seconds
    int phase = (now / 1000) % 60;

    bool machineRunning = !(phase >= 40 && phase < 50);

    // Simulated normal sensor readings
    float temperature =
        35.0 + 2.0 * sin(now / 7000.0);

    float vibration =
        0.35 + 0.08 * sin(now / 1300.0);

    // Test condition 1: overheating
    if (phase >= 20 && phase < 30) {
      temperature = 68.0;
    }

    // Test condition 2: excessive vibration
    if (phase >= 30 && phase < 40) {
      vibration = 3.2;
    }

    // Test condition 3: stopped machine
    if (!machineRunning) {
      temperature = 32.0;
      vibration = 0.05;
    }

    // Test condition 4: both faults
    if (phase >= 50) {
      temperature = 65.0;
      vibration = 3.0;
    }

    bool overTemperature =
        temperature > TEMP_LIMIT;

    bool highVibration =
        vibration > VIBRATION_LIMIT;

    bool alarm = machineRunning &&
                 (overTemperature || highVibration);

    // Turn on the LED when a running machine
    // has an abnormal reading
    digitalWrite(LED_PIN, alarm ? HIGH : LOW);

    // Print monitoring results
    Serial.println("-------------------------------");

    Serial.printf("Temperature: %.1f C\n",
                  temperature);

    Serial.printf("Vibration: %.2f g (simulated)\n",
                  vibration);

    Serial.printf("Machine: %s\n",
                  machineRunning ? "RUNNING" : "STOPPED");

    if (!machineRunning) {
      Serial.println("STATUS: MACHINE STOPPED");
    } else if (overTemperature && highVibration) {
      Serial.println("ALERT: HIGH TEMPERATURE + VIBRATION");
    } else if (overTemperature) {
      Serial.println("ALERT: HIGH TEMPERATURE");
    } else if (highVibration) {
      Serial.println("ALERT: HIGH VIBRATION");
    } else {
      Serial.println("STATUS: NORMAL");
    }
  }
}
