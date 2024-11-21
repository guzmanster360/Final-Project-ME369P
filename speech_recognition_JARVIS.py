# ME 369P Final Project
import speech_recognition as sr
import time
import serial
from gtts import gTTS
import os

port_name = "/dev/cu.usbmodem101"
# Replace 'COM3' with your Arduino's port (check the Arduino IDE or Device Manager)
arduino = serial.Serial(port=port_name, baudrate=115200, timeout=1)

time.sleep(2)  # Wait for the Arduino to initialize

def send_command(command):
    """
   Sends a command to the Arduino via the serial port.

   Parameters:
       command (str): The command string to send to the Arduino.
   """
    arduino.write(command.encode())  # Send command to Arduino
    time.sleep(0.1)  # Allow time for processing

def filter_text(text):
    """
    Filters and processes text for defined directional commands, extracting rotation degrees and direction.
    Sends appropriate commands to the Arduino and triggers text-to-speech feedback.

    Parameters:
        text (str): The spoken text input to be filtered and processed.

    """
    # Define the list of directional words to filter for
    directional_words = {"clockwise", "counterclockwise", "hover"}
    command =["", ""]
    for word in text.split():
      if word.isdigit():
          command[0] = word
      if word.lower() in directional_words:
          command[1] = word
          
    if command[1] == "clockwise" and command[0]:
        str_command = "-" + command[0]
        send_command(str_command)
    elif command[1] == "counterclockwise" and command[0]:
        str_command = command[0]
        send_command(str_command)
    elif command[1] == "hover":
        send_command("1")
        
    real_time_text_to_speech(command)
    
def real_time_text_to_speech(command):
    """
   Converts a command into real-time audio feedback using Google Text-to-Speech (gTTS).

   Parameters:
       command (list): A list containing rotation degrees and direction as strings.

   """
    if command[1] == "hover":
        tts = gTTS("Beginning hovering sequence", lang='en')
        tts.save("speech_hov.mp3")
        os.system("open speech_hov.mp3")
        print("Beginning hovering sequence.")
        
    elif command[0] != "" and command[1] != "":
        text = f"Now rotating {command[0]} degrees {command[1]}"
        tts = gTTS(text, lang='en')
        tts.save("speech_dir.mp3")
        os.system("open speech_dir.mp3")
        print(f"Now rotating {command[0]} degrees {command[1]}")
        
    elif command[1] == "" or command[0] == "":
        tts = gTTS("Incomplete command.", lang='en')
        tts.save("speech_incomplete.mp3")
        os.system("open speech_incomplete.mp3")
        print("Incomplete command.")
             
def real_time_speech_to_text():
    """
    Captures real-time speech input, transcribes it using Google Speech Recognition, 
    and processes the transcription for valid commands. Provides feedback on unrecognized input.

   """
    recognizer = sr.Recognizer()

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
                filter_text(corrected_text)
                repeat = input("\nNext Command? (y/n): ").strip().lower()
                if repeat != "y":
                    break
                    
            except sr.UnknownValueError:
                tts = gTTS("Sorry, I did not understand that.", lang='en')
                tts.save("speech_failure.mp3")
                os.system("open speech_failure.mp3")
                print("Sorry, I did not understand that.\n")
                time.sleep(1)
                
            except sr.RequestError:
                print("Could not request results from Google Speech Recognition service.")
            
            except sr.WaitTimeoutError:
                print("Timeout running again")

            except KeyboardInterrupt:
                print("\nExiting real-time transcription.")
                break
            
def main():   
    # Run the real-time speech-to-text function and save the result
    real_time_speech_to_text()
    
main()
