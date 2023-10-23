//this sketch is to communicate between 2 Mc using UART
//it will control servo motor position based on the incoming value from the other Mc
// it will send Button state to other Mc

//-----LIBRARIES------//
#include <SoftwareSerial.h> // include this library to use external UART communication
#include <Servo.h>   // include this library to control with servo

//----DEFINITIONS-----//
#define SERVO_MOTOR 9 // servo motor connected to pin D9
#define PUSH_BUTTON 5 // push button connected to pin D5 
#define UART_TX 2 // PIN D2 will be for UART transmition
#define UART_RX 3 // PIN D3 will be for UART recieve 
#define BUTTON_PRESSED LOW 
#define BUTTON_RELEASED HIGH

//---INSTANCES----//
SoftwareSerial EVER_Serial(UART_RX, UART_TX); // initialize an instance for Serial comm
Servo EVER_Servo; // initialize an instance for Servo 
 
//---------------//
int servoPosition_current = 0;
int servoPosition_previous = 0;
char buttonState = BUTTON_RELEASED;

void setup() {
  // put your setup code here, to run once:
  EVER_Serial.begin(9600); // setup communication with baud rate 9600
  EVER_Servo.attach(SERVO_MOTOR); // attach servo motor to pin D9 
  pinMode(PUSH_BUTTON, INPUT_PULLUP); //connect push button with internal pull up
  pinMode(SERVO_MOTOR, OUTPUT);
  EVER_Servo.write(servoPosition_current); 
}

void loop() {
  // put your main code here, to run repeatedly:
  // get servo required position from other Mc
  if (EVER_Serial.available()) {
    servoPosition_current = EVER_Serial.read();
  }
  // check if the servo position need to be changed and to position (0~180)
  if (servoPosition_current != servoPosition_previous) {
    if (servoPosition_current >= 0 && servoPosition_current <= 180) {
      EVER_Servo.write(servoPosition_current);
    }
  }
  servoPosition_previous = servoPosition_current;
  // toggle the led based on the button state sent from other Mc
  if (digitalRead(PUSH_BUTTON) == BUTTON_PRESSED) {
    delay(50);
    if (digitalRead(PUSH_BUTTON) == BUTTON_PRESSED) {
      buttonState = BUTTON_PRESSED;
    }
  } else {
    buttonState = BUTTON_RELEASED;
  }
  EVER_Serial.write(buttonState);
  delay(100);
}
