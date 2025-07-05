import serial
import time

ARDUINO_PORT = "COM17"
BAUD_RATE = 9600
TIMEOUT = 1

try:
    arduino = serial.Serial(ARDUINO_PORT, BAUD_RATE, timeout=TIMEOUT)
    time.sleep(2)  # Allow time for Arduino to reset
    print("Serial connection established.")
except serial.SerialException as e:
    print(f"Failed to connect to Arduino: {e}")
    arduino = None

def send_to_arduino(message: str):
    if arduino is not None:
        try:
            # message = message.strip() + "\n"
            arduino.write(message.encode('utf-8'))  # Send string
            print(f"Sent: {message}")
        except serial.SerialException as e:
            print(f"Serial error: {e}")

