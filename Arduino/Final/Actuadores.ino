/* Output-side (LED) Arduino code */
#include "SoftwareSerial.h"
// RX: Arduino pin 2, XBee pin DOUT.  TX:  Arduino pin 3, XBee pin DIN
// SoftwareSerial XBee(3, 4);
int LED_TEMP = 9;
int LED_LUM = 8;
int INA = 6;
int INB = 7;

int DIRA_MOTOR = 2;
int PWMA_MOTOR = 5;

byte speed = 80; // change this to alter speed 0 - 255 although 80 is the minimum for reliable operation

void encenderVentilador();
void apagarVentilador();
void encenderMotor();
void apagarMotor();


void setup()
{
  Serial.begin(9600);
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_LUM, OUTPUT);
  
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);
  
  pinMode(DIRA_MOTOR, OUTPUT);
  pinMode(PWMA_MOTOR, OUTPUT);
  

  
  
}
int estado=1;
int fin=0;
char valor;
void loop()
{
  
  apagarVentilador();
  apagarMotor();
  digitalWrite(LED_TEMP, LOW);
  digitalWrite(LED_LUM, LOW);
  // if (XBee.available()) {  
  if (Serial.available() > 0) {
    char c = Serial.read();
    switch(estado)
    {
      case 1:
        fin=0;
        valor=' ';
        if(c=='[')
          estado=2;
        break;
      case 2:
        if(c=='L' || c=='T')
        {
          estado=3;
          valor=c;
        }
        else
        {
          estado=1;
          fin=0;
        }
        break;
      case 3:
        if(c==']')
          fin=1;
        else
          fin=0;
        estado=1;
        break;
      default:
        break;
    }
    
    if(fin==1)
    {
      if(valor=='L')
      {
        encenderVentilador();
        digitalWrite(LED_LUM, HIGH);
        delay(1000);
      }
      else if(valor=='T')
      {
        encenderMotor();
        digitalWrite(LED_TEMP, HIGH);
        delay(1000);
      }
    }
  }
    // char c = XBee.read();
  /*String c = Serial.readString();
  Serial.println();
  if(c[0] == '[' && c[2]==']'){
    if(c[1]=='L')
     {
        encenderVentilador();
        analogWrite(LED_LUM, 255);
        delay(1000);
      }
      
  
   }
   if(c[0] == '[' && c[2]==']'){
    if(c[1]=='T')
     {
        encenderMotor();
        analogWrite(LED_TEMP, 255);
        delay(1000);
      }
      
  
   }*/
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


