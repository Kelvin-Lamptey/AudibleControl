**Audible Control**
This is a simple project I made with python and Arduino c++ that uses python to recognise spoken words and convert them to text which is fed to an llm via Groq to translate into commands which are sent to a dev board like the Arduino Uno via serial connection to perform certain functions on the board.

**Some of these functions include**
 * Turning on and off 6 LEDs
 * Changing the light of a traffic light module
 * Rotating a servo motor
 * Writing custom text on an LCD
 * Making sound with a buzzer

**Possibilties and Future plans**
This project could essentially be made to control a larger range of devices and components.
It currently has hard coded logic and I plan on making one with similar functionality as how Agentic AI is built for more custom functions as well.
I also plan on using openai-whisper for better voice recognition, I failed to use that in this project because my GPU could not run it and using the CPU could not produce any helpful results so I may try the API next time.
