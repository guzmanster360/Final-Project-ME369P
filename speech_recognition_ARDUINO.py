# ME 369P Final Project
import speech_recognition as sr
# import pyaudio
import time
import serial
from gtts import gTTS
import os

def send_command(command):
    arduino.write(command.encode())  # Send command to Arduino
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
          
    if command[0] != "" and command[1] != "":
        send_command(command)
    if command[1] == "hover":
        command = "hover"
        send_command(command)
    return command
    
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
                
                if filter_text(corrected_text) != ["", ""]:
                    all_text.append(filter_text(corrected_text)) 
                
                repeat = input("Next Command? (y/n): ").strip().lower()
                
                if repeat != "y":
                    break
            except sr.UnknownValueError:
                print("Sorry, I did not understand that.")
            except sr.RequestError:
                print("Could not request results from Google Speech Recognition service.")
            except KeyboardInterrupt:
                print("\nExiting real-time transcription.")
                break
    return all_text  # Return the accumulated transcription of directional words only

def main():   

    port_name = "/dev/cu.usbmodem101"
    arduino = serial.Serial(port=port_name, baudrate=115200, timeout=1)
    time.sleep(2)  # Wait for the Arduino to initialize
    
    # Run the real-time speech-to-text function and save the result
    transcribed_directional_text = real_time_speech_to_text()
    print("Directional Transcription: ")
   
    for index, inner_list in enumerate(transcribed_directional_text):
         if inner_list == "hover":
             tts = gTTS("Beginning hovering sequence", lang='en')
             tts.save("speech_hov.mp3")
             os.system("start speech_hov.mp3")
             send_command(inner_list)
             print(f"Command {index+1} : Hover")
         elif inner_list[0] != "" and inner_list[1] != "":
             text = f"Now moving {transcribed_directional_text[index][0]} degrees {transcribed_directional_text[index][0]}"
             tts = gTTS(text, lang='en')
             tts.save("speech_dir.mp3")
             os.system("start speech_dir.mp3")
             send_command(inner_list)
             print(f"Command {index+1} : {transcribed_directional_text[index][0]} degrees {transcribed_directional_text[index][1]}")
main()
