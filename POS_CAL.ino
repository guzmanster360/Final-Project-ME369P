#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#include <Servo.h>

// TIME INTERVAL SETUP
unsigned long previousMillis = 0;  // Variable to store the time of the last sensor reading
unsigned long currentMillis = 0;   // Variable to store the current time
unsigned long dt = 0;

LSM6DSO myIMU;  //I2C, ADDR 0x6Bs
float z_accel = 0;
float z_vel = 0;
float z_pos = 0;
float z_targ = 0;

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  delay(5000); // SET TO 0 POS
}

void loop() {
  Serial.println(accel_int());
}

float accel_int() {

  currentMillis = millis();  // Get the current time in milliseconds

  // Read the sensor if sufficient time has passed
  if (currentMillis - previousMillis >= 5) {  // 10ms interval (for example)

    previousMillis = currentMillis;  // Update previousMillis to the current time

    Serial.println(myIMU.readFloatAccelZ(), 3);
    z_accel = myIMU.readFloatAccelZ()
    dt = currentMillis - previousMillis;
    z_vel += z_accel * dt;
    z_pos += z_vel * dt;

    return z_pos;
  }
}