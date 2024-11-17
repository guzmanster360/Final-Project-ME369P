#include <Tlv493d.h>
#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#include <Servo.h>

// PID SETUP
const float P = 0.05;
const float I = 1.2;
const float D = 0.05;
float integral = 0;
float previous_error = 0;
float output = 0;
unsigned long previousMillis = 0;  // Variable to store the time of the last sensor reading
unsigned long currentMillis = 0;   // Variable to store the current time
unsigned long dt = 0;

// STEPPER SETUP
const int dirPin = 7;
const int stepPin = 6;
const int stepsPerRevolution = 200;

int dir = 0;
int step = 0;
int speed = 0;

// MAGNETOMETER PRE SETUP
Tlv493d Tlv493dMagnetic3DSensor = Tlv493d();
float z_mag = 0;
int ang = 0;

// ACCEL SETUP
LSM6DSO myIMU;  //I2C, ADDR 0x6Bs
float z_accel = 0;
float z_vel = 0;
float z_pos = 0;
float z_targ = 0;

// EDF PRE SETUP
int edfPin = 11;
int thrust = 0;
Servo edf;

void setup() {
  Serial.begin(9600);   // Initialize serial communication
  pinMode(13, OUTPUT);  // Set pin 13 as output
  pinMode(edfPin, OUTPUT);

  // SETUP FOR STEPPER MOTOR
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // SETUP FOR EDF FAN
  edf.attach(edfPin);
  edf.writeMicroseconds(1100);
  delay(5000);

  // SETUP SERIAL FOR MAGNETOMETER
  while (!Serial)
    ;
  Tlv493dMagnetic3DSensor.begin();
  Tlv493dMagnetic3DSensor.setAccessMode(Tlv493dMagnetic3DSensor.MASTERCONTROLLEDMODE);
  Tlv493dMagnetic3DSensor.disableTemp();

  move(false, 55, 4000, 1320);
}

void loop() {

  // PYTHON CONNECTION
  // if (Serial.available() > 0) { // Check if data is received
  //   char command = Serial.read(); // Read the command
  //   if (command == 'H') {
  //     digitalWrite(13, HIGH); // Turn LED on
  //   } else if (command == 'L') {
  //     digitalWrite(13, LOW); // Turn LED off
  //   }
  // }

  // RETRIEVE ACCELEROMETER VALUE
  Serial.print(", Z = ");
  Serial.println(myIMU.readFloatAccelZ(), 3);

  // RETRIEVE MAGNETOMETER VALUE
  delay(Tlv493dMagnetic3DSensor.getMeasurementDelay());
  Tlv493dMagnetic3DSensor.updateData();
  z_mag = Tlv493dMagnetic3DSensor.getZ();

  int pwm = map(z_mag, -128.5, 127, 1100, 1500);
  // edf.writeMicroseconds(pwm);
  Serial.println(pwm);
}


int step_mot(int dir, int step, int speed) {

  if (dir == true) {
    // CLOCKWISE
    digitalWrite(dirPin, HIGH);
  } else {
    // COUNTERCLOCKWISE
    digitalWrite(dirPin, LOW);
  }
  // SPIN MOTOR
  for (int x = 0; x < step; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
  delay(1000);  // Wait a second
}

void move(int rot, int step, int speed, int thrust) {

  // MOVE RIGHT
  if (rot == true) {
    // COUNTERCLOCKWISE
    digitalWrite(dirPin, HIGH);
  } else {
    // CLOCKWISE
    digitalWrite(dirPin, LOW);
  }
  // SPIN MOTOR
  for (int x = 0; x < step; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
  delay(500);  // Wait a second

  delay(Tlv493dMagnetic3DSensor.getMeasurementDelay());
  Tlv493dMagnetic3DSensor.updateData();

  z_mag = Tlv493dMagnetic3DSensor.getZ() + 128.5;
  ang = map(z_mag, 0, 255.5, 0, 360);
  Serial.println(ang);


  while (ang > 2) {

    delay(Tlv493dMagnetic3DSensor.getMeasurementDelay());
    Tlv493dMagnetic3DSensor.updateData();
    z_mag = Tlv493dMagnetic3DSensor.getZ() + 128.5;
    ang = map(z_mag, 0, 255.5, 0, 360);

    Serial.print(ang);
    Serial.print(" ");
    Serial.println(z_mag);
    edf.writeMicroseconds(thrust);
  }
  edf.writeMicroseconds(1100);
}

void hover() {
  // Calculate error

  float error = z_targ - accel_int();

  // Proportional term
  float P_term = P * error;

  // Integral term
  integral += error;
  float I_term = I * integral;

  // Derivative term
  float D_term = D * (error - previous_error);

  // Calculate total PID output
  output = P_term + I_term + D_term;

  // Save the current error for the next iteration
  previous_error = error;

  int pwm = map(output, -1, 1, 1100, 1500);
}

float accel_int() {

  currentMillis = millis();  // Get the current time in milliseconds

  // Read the sensor if sufficient time has passed
  if (currentMillis - previousMillis >= 5) {  // 10ms interval (for example)
    
    previousMillis = currentMillis;  // Update previousMillis to the current time

    Serial.println(myIMU.readFloatAccelZ(), 3);
    z_accel = myIMU.readFloatAccelZ();
    // Calculate the sample time (time elapsed since the last reading)
    dt = currentMillis - previousMillis;
    z_vel += z_accel * dt;
    z_pos += z_vel * dt; 
    
    return z_pos;
  }
}