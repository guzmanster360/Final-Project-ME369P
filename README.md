# **ME 369P Final Project: J.A.R.V.I.S for an Electric Thruster**

## **Project Description**
This project is a Python-based system that integrates **speech recognition**, **text-to-speech (TTS)** capabilities, and Arduino control. The application enables users to issue voice commands to control an electric through an Arduino, allowing real-time interaction using natural language inputs.

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
