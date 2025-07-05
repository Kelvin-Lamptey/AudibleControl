#include <Servo.h>
#include <LCD-I2C.h>
#include <Wire.h>

int servoPin = 3; 
int LED1=2,LED2=4,LED3=7,LED4=8,LED5=12,LED6=13;
int TRAFFICRED=5,TRAFFICYELL=6,TRAFFICGREEN = 11;
int buzzerPin = 10;
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according
Servo KServo;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  KServo.attach(servoPin);
  //for the lcd
  Wire.begin();
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(TRAFFICRED, OUTPUT);
  pinMode(TRAFFICYELL, OUTPUT);
  pinMode(TRAFFICGREEN, OUTPUT);
}

void writeLCD( String text)
{
  lcd.clear();
  lcd.print( text);
}

void turnServo(float angle)
{
  KServo.write(0);
  KServo.write(angle); 
}

void soundBuzzer(int freq)
{
  tone( buzzerPin, freq, 2000);
}

void turnOnLED(int pinNum)
{
  digitalWrite(pinNum, HIGH);
}
void turnOffLED(int pinNum)
{
  digitalWrite(pinNum, LOW);
}
void trafficLight(int color){
  if(color == 1){
    digitalWrite(TRAFFICRED,HIGH);
    digitalWrite(TRAFFICYELL,LOW);
    digitalWrite(TRAFFICGREEN,LOW);
  }else if(color == 2){
    digitalWrite(TRAFFICRED,LOW);
    digitalWrite(TRAFFICYELL,HIGH);
    digitalWrite(TRAFFICGREEN,LOW);
  }else {
    digitalWrite(TRAFFICRED,LOW);
    digitalWrite(TRAFFICYELL,LOW);
    digitalWrite(TRAFFICGREEN,HIGH);
  }
}
void loop() {
  // // Make servo go to 0 degrees 
  //  KServo.write(0); 
  //  delay(1000); 
  //  // Make servo go to 90 degrees 
  //  KServo.write(90); 
  //  delay(1000); 
  //  // Make servo go to 180 degrees 
  //  KServo.write(180); 
  //  delay(1000); 
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); // Read until newline character
    command.trim(); // Remove any leading/trailing whitespace
    
    if (command == nullptr || command == "") { // Check if "command" key exists
      Serial.println("No 'command' key found in string.");
      return;
    }
  
    String commandsToExecute = String(command);
    Serial.println("Commands to execute: " + commandsToExecute);

    // Split commands by comma
    int commandCount = 0;
    int prevIndex = 0;
    int currentIndex = commandsToExecute.indexOf(',');

    while (currentIndex >= 0) {
      String singleCommand = commandsToExecute.substring(prevIndex, currentIndex);
      parseAndExecuteCommand(singleCommand);

      prevIndex = currentIndex + 1;
      currentIndex = commandsToExecute.indexOf(',', prevIndex);
      commandCount++;
    }
    // Process the last command (or the only command if no commas)
    String lastCommand = commandsToExecute.substring(prevIndex);
    parseAndExecuteCommand(lastCommand);
    commandCount++;

    Serial.println("Processed " + String(commandCount) + " commands.");
  }
}

// --- Function to parse and execute a single command string ---
void parseAndExecuteCommand(String cmd) {
  cmd.trim(); // Remove leading/trailing whitespace
  if (cmd.length() == 0) {
    return; // Skip empty commands
  }
  Serial.println("Parsing command: " + cmd);

  // LED Commands: led<1-7>:<0|1>
  if (cmd.startsWith("led")) {
    int colonIndex = cmd.indexOf(':');
    if (colonIndex != -1) {
      int ledNum = cmd.substring(3, colonIndex).toInt(); // Extract LED number
      int state = cmd.substring(colonIndex + 1).toInt(); // Extract state (0 or 1)

      int actualLedPin = -1;
      switch (ledNum) {
        case 1: actualLedPin = LED1; break;
        case 2: actualLedPin = LED2; break;
        case 3: actualLedPin = LED3; break;
        case 4: actualLedPin = LED4; break;
        case 5: actualLedPin = LED5; break;
        case 6: actualLedPin = LED6; break;
        // If you had LED7 defined, you'd add it here
        default: actualLedPin = -1; break; 
      }

      if (actualLedPin != -1) {
        if (state == 1) {
          turnOnLED(actualLedPin);
        } else if (state == 0) {
          turnOffLED(actualLedPin);
        } else {
          Serial.println("Invalid LED state: " + String(state));
        }
      } else {
        Serial.println("Invalid LED number: " + String(ledNum));
      }
    } else {
      Serial.println("Malformed LED command: " + cmd);
    }
  }
  // LCD Command: lcd <text>
  else if (cmd.startsWith("lcd ")) {
    String text = cmd.substring(4); // Get text after "lcd "
    writeLCD(text);
  }
  // Traffic Light Command: traffic:<1=red,2=yellow,3=green>
  else if (cmd.startsWith("traffic:")) {
    int color = cmd.substring(8).toInt(); // Get color value after "traffic:"
    trafficLight(color);
  }
  // Servo Command: servo <angle>
  else if (cmd.startsWith("servo ")) {
    int angle = cmd.substring(6).toFloat(); // Get angle after "servo "
    if (angle >= 0 && angle <= 180) { // Validate angle for servo
      // turnServo(angle);
      KServo.write(angle);
    } else {
      writeLCD("Invalid servo angle: " + String(angle));
    }
  }
  // Buzzer Command: buzzer <1-1000 freq>
  else if (cmd.startsWith("buzzer ")) {
    writeLCD("Buzzing");
    int freq = cmd.substring(7).toInt(); // Get frequency after "buzzer "
    soundBuzzer(freq);
  }
  else {
    Serial.println("Unknown command: " + cmd);
    writeLCD("Error");
  }
}

