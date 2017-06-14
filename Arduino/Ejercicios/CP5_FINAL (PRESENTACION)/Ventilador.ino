/*********************************************************
* Fan Driver using L9110 Motor Driver
*
* The L9110 Motor Diver is a simple driver designed for
* toys and robots. It has two inputs INA and INB
* and two outputs to drive a simple DC Motor.
*
* The direction of motor spin can be changed and the speed
* can be controlled using PWM
*
* The device described here is the Keyes Fan Board with
* 4 connections
*
* Connections:
*
* INA to Arduino PIN 5
* INB to Arduino PIN 6
* Vcc to Arduino 5 volts
* GND to Arduino GND
*
* Push Button:
* (the output went LOW on pressing on my unit)
* Gnd to Arduino Gnd
* Vcc to Arduino 5 volts
* S to Adduino PIN 7
*
*******************************************************/
const int INA = 5;
const int INB = 6;

byte speed = 255; // change this to alter speed 0 - 255 although 80 is the minimum for reliable operation

void setup()
{
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);
  Serial.begin(9600); // for debug
}

void loop()
{
  analogWrite(INA,speed);
  analogWrite(INB,0);
  delay(2000);
  digitalWrite(INA,LOW);
  digitalWrite(INB,LOW);
  delay(2000);
}
