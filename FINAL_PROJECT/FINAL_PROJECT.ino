#include <Tlv493d.h>
#include "SparkFunLSM6DSO.h"
#include "Wire.h"
#include <Servo.h>

int command;  // Create a String to store incoming data

// PID SETUP
const float P = 0.5;
const float I = 0.0001;
const float D = 10.00;
float integral = 0;
float previous_error = 0;
float output = 0;
unsigned long previousMillis = 0;  // Variable to store the time of the last sensor reading
float z_targ = -25;


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
float left_min = -199.00;
float left_max = 155.00;
float ang = 0;

// ACCEL SETUP
LSM6DSO myIMU;  //I2C, ADDR 0x6Bs
float x_vel = 0;
float z_ang = 0;
float alpha = 0.2;     // Smoothing factor (0 < alpha <= 1)
float rawgyroX = 0.0;  // Raw acceleration data
float filteredgyroX = 0.0;

// EDF PRE SETUP
int edfPin = 5;
int thrust = 0;
Servo edf;

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  pinMode(13, OUTPUT);   // Set pin 13 as output
  pinMode(edfPin, OUTPUT);

  // SETUP FOR STEPPER MOTOR
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // SETUP FOR EDF FAN
  edf.attach(edfPin);
  edf.writeMicroseconds(1100);
  delay(5000);

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

  // SETUP SERIAL FOR MAGNETOMETER
  while (!Serial)
    ;
  Tlv493dMagnetic3DSensor.begin();
  Tlv493dMagnetic3DSensor.setAccessMode(Tlv493dMagnetic3DSensor.MASTERCONTROLLEDMODE);
  Tlv493dMagnetic3DSensor.disableTemp();

  move(false, 55, 4000, 1250);
}

void loop() {

  // PYTHON CONNECTION
  // command = Serial.readString().toInt();  // Read one byte
  // Serial.println(command);
  // if (command == 1) {
  //   Serial.println("MADE IT");
  //   unsigned long startTime = millis();
  //   while (millis() - startTime < 30000) {
  //     hover(gyro_int());
  //   }
  // }
  // hover(gyro_int());
  // else if (incomingData == ) {


  // RETRIEVE ACCELEROMETER VALUE
  // Serial.print(", Z = ");
  // Serial.println(myIMU.readFloatAccelZ(), 3);

  // RETRIEVE MAGNETOMETER VALUE
  delay(Tlv493dMagnetic3DSensor.getMeasurementDelay());
  Tlv493dMagnetic3DSensor.updateData();
  z_mag = Tlv493dMagnetic3DSensor.getZ();
  Serial.println(z_mag);

  // int pwm = map(z_mag, -128.5, 127, 1100, 1500);
  // // edf.writeMicroseconds(pwm);
  // Serial.println(pwm);
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

  z_mag = Tlv493dMagnetic3DSensor.getZ();
  Serial.println(abs(z_mag));


  while (z_mag > left_min) {

    delay(Tlv493dMagnetic3DSensor.getMeasurementDelay());
    Tlv493dMagnetic3DSensor.updateData();
    z_mag = Tlv493dMagnetic3DSensor.getZ();
    edf.writeMicroseconds(thrust);
  }
  edf.writeMicroseconds(1100);
  delay(500);
  // RETURN TO OG POS
  if (rot == true) {
    // COUNTERCLOCKWISE
    digitalWrite(dirPin, LOW);
  } else {
    // CLOCKWISE
    digitalWrite(dirPin, HIGH);
  }
  for (int x = 0; x < step; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(speed);
  }
  delay(500);  // Wait a second
}

void hover(float gyro) {
  // Calculate error
  // Serial.println(gyro);
  float error = z_targ - gyro;

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
  Serial.println(abs(output));
  // Serial.print(" ");
  int pwm = map(abs(output), 0, 350, 1100, 1500);
  edf.writeMicroseconds(pwm);
}

float gyro_int() {

  unsigned long currentMillis = millis();  // Get the current time in milliseconds

  // Read the sensor if sufficient time has passed
  if (currentMillis - previousMillis >= 10) {  // 10ms interval (for example)
    float dt = (currentMillis - previousMillis) / 1000.0;
    // Serial.println(dt, 15);
    previousMillis = currentMillis;  // Update previousMillis to the current time


    rawgyroX = (myIMU.readFloatGyroX());
    filteredgyroX = lowPassFilter(rawgyroX, filteredgyroX, alpha);

    z_ang += filteredgyroX * dt;

    return z_ang;
  }
}

float lowPassFilter(float currentValue, float previousFilteredValue, float alpha) {
  // alpha is the smoothing factor: 0 < alpha <= 1
  return alpha * currentValue + (1 - alpha) * previousFilteredValue;
}