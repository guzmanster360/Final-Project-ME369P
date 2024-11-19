import serial
import time
import speech_recognition as sr

recognizer = sr.Recognizer()

port_name = ''
# Replace 'COM3' with your Arduino's port (check the Arduino IDE or Device Manager)
arduino = serial.Serial(port=port_name, baudrate=9600, timeout=1)

time.sleep(2)  # Wait for the Arduino to initialize

def send_command(command):
    arduino.write(command.encode())  # Send command to Arduino
    time.sleep(0.1)  # Allow time for processing

# Example usage
send_command('H')  # Turn LED on
time.sleep(2)
send_command('L')  # Turn LED off

def speak():
    with sr.Microphone() as source:
        print("Say something!")
        audio = recognizer.listen(source)
        while audio != 'quit':
            audio = recognizer.listen(source)
            if audio == 'right':
                send_command(audio)
            elif audio == 'left':
                send_command(audio)
            elif audio == 'hover':
                send_command(audio)
speak()