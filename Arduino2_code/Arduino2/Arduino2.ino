//this sketch is to communicate between 2 Mc using UART
//it will toggle led based on the button state coming from other Mc

//-----LIBRARIES------//
#include <SoftwareSerial.h>  // include this library to use external UART communication
#include <Servo.h>           // include this library to control with servo

//----DEFINITIONS-----//
#define POTENTIOMETER A0  // poteniometer connected to Pin A0 on arduino
#define LED 12            // LED connected to Pin D12 on arduino
#define UART_TX 2         // PIN D2 will be for UART transmition
#define UART_RX 3         // PIN D3 will be for UART Receive
#define BUTTON_PRESSED LOW


//---INSTANCES----//
SoftwareSerial EVER_Serial(UART_RX, UART_TX);  // initialize an instance for Serial comm
//---------------//
int servoPosition_previous = 0; 
int servoPosition_current = 0;
void setup() {
  // put your setup code here, to run once:
  EVER_Serial.begin(9600);  // setup communication with baud rate 9600
  pinMode(LED, OUTPUT); 
  pinMode(POTENTIOMETER, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  servoPosition_current = map(analogRead(POTENTIOMETER), 0, 1023, 0, 180); // mapping the value of poteniometer to get a position for servo (0`180)
  // check if the poteniometer value changed then send it to other Mc
  if (servoPosition_current != servoPosition_previous) {
    EVER_Serial.write(servoPosition_current);
  }
  servoPosition_previous = servoPosition_current;
  // recieve button state from other Mc and toggle the led based on it
  if (EVER_Serial.available()) {
    EVER_Serial.read() == BUTTON_PRESSED ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
  }
  delay(100);
}