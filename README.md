# Final-Project-ME369D

ME 369P Final Project: Voice-Controlled Arduino System
Project Description
This project is a Python-based system that integrates speech recognition, text-to-speech (TTS) capabilities, and Arduino control. The application enables users to issue voice commands to control a connected device (e.g., a motor) through an Arduino, allowing real-time interaction using natural language inputs.

Features
Real-Time Speech Recognition
Uses the Google Speech Recognition API to process spoken commands.
Text Filtering
Filters commands for directional words like clockwise, counterclockwise, and hover.
Extracts numerical degrees for motor rotation.
Arduino Integration
Sends commands to an Arduino via a serial port for motor control.
Text-to-Speech Feedback
Provides audio feedback on recognized commands or errors.
Interactive User Flow
Includes user-friendly prompts to process multiple commands in sequence.
Project Structure
plaintext
Copy code
.
├── main.py              # Main script for running the system
├── requirements.txt     # Python dependencies
├── README.md            # Project documentation
├── speech_hov.mp3       # Hovering sequence audio file (generated at runtime)
├── speech_dir.mp3       # Directional command audio file (generated at runtime)
└── speech_incomplete.mp3# Incomplete command audio file (generated at runtime)
Installation
Prerequisites
Python 3.x
Arduino IDE (for configuring the Arduino board)
An Arduino board connected to your computer (ensure the correct port is specified in the code).
Steps
Clone this repository:
bash
Copy code
git clone https://github.com/yourusername/voice-controlled-arduino.git
Navigate to the project directory:
bash
Copy code
cd voice-controlled-arduino
Install the required Python packages:
bash
Copy code
pip install -r requirements.txt
Verify the Arduino port:
Update port_name in the code to match your Arduino's port (e.g., /dev/cu.usbmodem101 or COM3).
Usage
Connect the Arduino to your computer via USB.
Run the main script:
bash
Copy code
python main.py
Speak commands like:
"Rotate 90 degrees clockwise"
"Rotate 45 degrees counterclockwise"
"Hover"
Follow the prompts to issue additional commands or exit the program.
Command Breakdown
Directional Words:
clockwise: Negative rotation command sent to Arduino.
counterclockwise: Positive rotation command sent to Arduino.
hover: Special command to initiate hovering sequence.
Text-to-Speech:
Audio feedback confirms or notifies users of incomplete commands.
Dependencies
SpeechRecognition
gTTS (Google Text-to-Speech)
pySerial
Install these using the command:

bash
Copy code
pip install -r requirements.txt
Arduino Setup
Upload a compatible sketch to your Arduino board that reads serial input and controls the motor accordingly.
Ensure the baud rate in the Arduino sketch matches 115200 in the Python script.
Project Links
GitHub Repository
Python SpeechRecognition Documentation
Contributing
Contributions are welcome! Please:

Fork this repository.
Create a feature branch.
Submit a pull request.
License
This project is licensed under the MIT License. See the LICENSE file for details.

Acknowledgments
Google Speech Recognition API for enabling voice-to-text capabilities.
gTTS for real-time audio feedback generation.
Arduino Community for hardware integration resources.
