#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#include <Servo.h>

// TIME INTERVAL SETUP
unsigned long previousMillis = 0;  // Variable to store the time of the last sensor reading

LSM6DSO myIMU;  //I2C, ADDR 0x6Bs
float x_vel = 0;
float z_ang = 0;
float z_targ = 0;

float alpha = 0.2;               // Smoothing factor (0 < alpha <= 1)
float rawgyroX = 0.0;           // Raw acceleration data
float filteredgyroX = 0.0;  

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  delay(5000); // SET TO 0 POS

  Wire.begin();
  delay(10);
  if( myIMU.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");
}

void loop() {
  Serial.println(accel_int(), 5);
  // accel_int();
}

float accel_int() {

  unsigned long currentMillis = millis();  // Get the current time in milliseconds

  // Read the sensor if sufficient time has passed
  if (currentMillis - previousMillis >= 10) {  // 10ms interval (for example)
    float dt = (currentMillis - previousMillis)/1000.0;
    // Serial.println(dt, 15);
    previousMillis = currentMillis;  // Update previousMillis to the current time

    
    rawgyroX = (myIMU.readFloatGyroX());
    filteredgyroX = lowPassFilter(rawgyroX, filteredgyroX, alpha);
    Serial.println(filteredgyroX);

    z_ang += filteredgyroX * dt;

    return z_ang;
  }
}
float lowPassFilter(float currentValue, float previousFilteredValue, float alpha) {
  // alpha is the smoothing factor: 0 < alpha <= 1
  return alpha * currentValue + (1 - alpha) * previousFilteredValue;
}