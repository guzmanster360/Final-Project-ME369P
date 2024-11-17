import serial
import time

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