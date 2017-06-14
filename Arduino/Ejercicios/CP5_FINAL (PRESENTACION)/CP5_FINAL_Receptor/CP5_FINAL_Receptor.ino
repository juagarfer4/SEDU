/* Output-side (LED) Arduino code */
#include "SoftwareSerial.h"
// RX: Arduino pin 2, XBee pin DOUT.  TX:  Arduino pin 3, XBee pin DIN
SoftwareSerial XBee(3, 4);
int LED_TEMP = 12;
int LED_LUM = 13;
const int INA = 6;
const int INB = 7;

const int DIRA_MOTOR = 2;
const int PWMA_MOTOR = 5;

byte speed = 80; // change this to alter speed 0 - 255 although 80 is the minimum for reliable operation

void encenderVentilador();
void apagarVentilador();
void encenderMotor();
void apagarMotor();


void setup()
{
  // Baud rate MUST match XBee settings (as set in XCTU)
  XBee.begin(9600);
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_LUM, OUTPUT);
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);

  pinMode(DIRA_MOTOR, OUTPUT);
  pinMode(PWMA_MOTOR, OUTPUT);
  
}

void loop()
{
  apagarVentilador();
  apagarMotor();
  digitalWrite(LED_TEMP, LOW);
  digitalWrite(LED_LUM, LOW);
  if (XBee.available())  
  { 
    char c = XBee.read();
    if (c == 'm')
    {
      digitalWrite(LED_TEMP, HIGH);
      // [MOTOR PRODUCTION VERSION] UNCOMMENT IN PRODUCTION
      // encenderMotor();
      delay(1000);
    }
    else if (c == 'l')
    {
      digitalWrite(LED_LUM, HIGH);
      encenderVentilador();
      // [MOTOR DEMO VERSION] REMOVE IN PRODUCTION
      encenderMotor();
      delay(1000);
    }
  }
  
      
  
}

void encenderVentilador()
{
  analogWrite(INA,speed);
  analogWrite(INB,0);
}

void apagarVentilador()
{
  digitalWrite(INA,LOW);
  digitalWrite(INB,LOW); 
}

void encenderMotor()
{
  digitalWrite(DIRA_MOTOR, HIGH);
  analogWrite(PWMA_MOTOR, 160);
}

void apagarMotor()
{
  digitalWrite(DIRA_MOTOR, LOW);
  analogWrite(PWMA_MOTOR, 0);
}


