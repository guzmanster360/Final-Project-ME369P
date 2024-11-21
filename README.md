# Final-Project-ME369D






You said:
write that in Github format
ChatGPT said:
ChatGPT
Here’s the README formatted for GitHub using Markdown syntax:

markdown
Copy code
# **ME 369P Final Project: Voice-Controlled Arduino System**

## **Project Description**
This project is a Python-based system that integrates **speech recognition**, **text-to-speech (TTS)** capabilities, and Arduino control. The application enables users to issue voice commands to control a connected device (e.g., a motor) through an Arduino, allowing real-time interaction using natural language inputs.

---

## **Features**
- **Real-Time Speech Recognition**  
  Utilizes the Google Speech Recognition API to process spoken commands.
- **Text Filtering**  
  Filters commands for directional words like `clockwise`, `counterclockwise`, and `hover`. Extracts numerical degrees for motor rotation.
- **Arduino Integration**  
  Sends commands to an Arduino via a serial port for motor control.
- **Text-to-Speech Feedback**  
  Provides audio feedback on recognized commands or errors.
- **Interactive User Flow**  
  Includes user-friendly prompts to process multiple commands in sequence.

---

## **Project Structure**
```plaintext
.
├── main.py              # Main script for running the system
├── requirements.txt     # Python dependencies
├── README.md            # Project documentation
├── speech_hov.mp3       # Hovering sequence audio file (generated at runtime)
├── speech_dir.mp3       # Directional command audio file (generated at runtime)
└── speech_incomplete.mp3# Incomplete command audio file (generated at runtime)
