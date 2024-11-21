# ME 369P Final Project
import speech_recognition as sr
import pyaudio
import time
import serial
from gtts import gTTS
import os

port_name = "/dev/cu.usbmodem101"
# Replace 'COM3' with your Arduino's port (check the Arduino IDE or Device Manager)
arduino = serial.Serial(port=port_name, baudrate=115200, timeout=1)
time.sleep(2)  # Wait for the Arduino to initialize

def send_command(command):
    arduino.write(bytes(command, 'utf-8'))   # Send command to Arduino
    time.sleep(0.1)  # Allow time for processing

def filter_text(text):
    
    # Define the list of directional words to filter for
    directional_words = {"clockwise", "counterclockwise", "hover"}
    
    command =["", ""]
    for word in text.split():
      if word.isdigit():
          command[0] = word
      if word.lower() in directional_words:
          command[1] = word
          

    if command[1] == "clockwise" and command[0]:
        int_command = int(command[0])
        int_command *= -1
        send_command(int_command)
        
    elif command[1] == "counterclockwise" and command[0]:
        int_command = int(command[0]) 
        send_command(int_command)
    
    if command[1] == "hover":
        int_command = 1
        send_command(int_command)
        
    real_time_text_to_speech(command)
    return command
    
def real_time_text_to_speech(command):
    if command[1] == "hover":
        tts = gTTS("Beginning hovering sequence", lang='en')
        tts.save("speech_hov.mp3")
        os.system("start speech_hov.mp3")
        print("Beginning hovering sequence.")
        
    elif command[0] != "" and command[1] != "":
        text = f"Now rotating {command[0]} degrees {command[1]}"
        tts = gTTS(text, lang='en')
        tts.save("speech_dir.mp3")
        os.system("start speech_dir.mp3")
        print(f"Now rotating {command[0]} degrees {command[1]}")
        
    elif command[1] == "" or command[0] == "":
        tts = gTTS("Incomplete command.", lang='en')
        tts.save("speech_incomplete.mp3")
        os.system("start speech_incomplete.mp3")
        print("Incomplete command.")
             
def real_time_speech_to_text():
    recognizer = sr.Recognizer()
    all_text = [] # Initialize an empty list to store all transcriptions

    # Use the default microphone as the audio source
    with sr.Microphone() as source:
        
        print("Adjusting for ambient noise... please wait.")
        recognizer.adjust_for_ambient_noise(source)
        print("Ready.")
        
        # Continuously listen and recognize speech
        while True:
            try:
                # Capture the audio
                print("Countdown: listening in ")
                for i in range(3, 0, -1):
                    print(f"{i}...")
                    time.sleep(1)
                print("Listening!")
                audio_data = recognizer.listen(source, timeout = 10)
                
                # Transcribe the audio using Google’s Web Speech API
                text = recognizer.recognize_google(audio_data)
                corrected_text  = text.replace("°", " degrees")
                print("You said: " + corrected_text)
                
                filtered_text = filter_text(corrected_text)
                if filtered_text != ["", ""]:
                    all_text.append(filtered_text) 
                
                repeat = input("\nNext Command? (y/n): ").strip().lower()
                
                if repeat != "y":
                    break
            except sr.UnknownValueError:
                tts = gTTS("Sorry, I did not understand that.", lang='en')
                tts.save("speech_failure.mp3")
                os.system("start speech_failure.mp3")
                print("Sorry, I did not understand that.\n")
                time.sleep(1)
                
            except sr.RequestError:
                print("Could not request results from Google Speech Recognition service.")
                
            except KeyboardInterrupt:
                print("\nExiting real-time transcription.")
                break
            
    return all_text  # Return the accumulated transcription of directional words only

def main():   
    # Run the real-time speech-to-text function and save the result
    transcribed_directional_text = real_time_speech_to_text()
    
main()