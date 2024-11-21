# **ME 369P Final Project: J.A.R.V.I.S for an Electric Thruster**

## **Project Description**
This project is a Python-based system that integrates **speech recognition**, **text-to-speech (TTS)** capabilities, and **Arduino control**. The application enables users to issue voice commands to control an electric thruster through an Arduino, allowing real-time interaction using natural language inputs. This is a custom electric thruster for our class presentation.

---
## **Electric Thruster**
This project features an electric thruster system with 3 degrees of freedom (roll, pitch, and yaw) of control. The supply sufficient power the system is powered by a 14.8V 100C battery and utilizes an Arduino Uno for precise control of a NEMA 17 stepper motor. The thruster is an electric ducted fan (EDF), driven by an electronic speed controller (ESC) which receives the PID output from system. The system is equipped with a magnetometer for rotational position monitoring and and a 6-axis inertial measurement unit (IMU) for angular velocity/position tracking.

For the hovering sequence a PID controller was implemented to maintain a constant thruster angular position (25 degrees for example). By integrating the X axis data for angular velocity in real time we can implement position control that adjusts the PWM signal sent to the ESC which would then influence the magnitude of the thrust produced by the EDF.

![image](https://github.com/user-attachments/assets/f524e32d-86da-4604-a554-ac3d68d083b7)

---
## **Features**
- **Real-Time Speech Recognition**  
  Utilizes the Google Speech Recognition API to process spoken commands.
- **Text Filtering**
  Filters commands for directional words like `clockwise`, `counterclockwise`, and `hover`. Extracts numerical degrees for electric thruster rotation.
- **Arduino Integration**  
  Sends commands to an Arduino via a serial port for electric thruster control.
- **Text-to-Speech Feedback**  
  Provides audio feedback on recognized commands or errors.
- **Interactive User Flow**  
  Includes user-friendly prompts to process multiple commands in sequence.
  
---
  ## **Packages Utilized**
  The following packages must be installed before use.
- **SpeechRecognition**
  Speech to text conversion using various APIs, we utilize Google Web Speech API.
  Handles failed recognition and connectivity issue errors.
- **PyAudio**
  Utilized by speech_recognition to capture real time audio input.
- **Serial**
  Allows for serial port connection to microcontrollers (Arduino).
- **gTTS (Google Text-to-Speech)**
  Google’s TTS API, converting strings to audio files.
  Multiple languages, accents, voices.
- **OS**
  Interact with operating system, used to play the TTS mp3 files in real time.

---
  ## **Authors**
  - Scott Gafka
  - Victor Guzman
  - Yan Zhe Wong







