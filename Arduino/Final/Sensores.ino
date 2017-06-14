// DHT sensor library - Version: Latest 

// http://hetpro-store.com/TUTORIALES/mcp9700-sensor-de-temperatura/

#include <DHT11.h>

#include "RTClib.h"

#include <LiquidCrystal.h>

/* Input-side (button) Arduino code */
#include "SoftwareSerial.h"

// Pin A4: SDA, A5: SCL
// Pin A2: Luminosidad

int pin_dht11 = 2;
int pin_photocell = A2;
int pin_RX = 3;
int pin_TX = 12;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int photocell_umbral = 500;
int temp_umbral = 28;

// RX: Arduino pin 2, XBee pin DOUT.  TX:  Arduino pin 3, XBee pin DIN
SoftwareSerial XBee(pin_RX, pin_TX);
DHT11 dht11(pin_dht11);
RTC_DS3231 rtc;

void mostrarLCD(float temp, float hum, int photocellReading, String d);
void mostrarSerial(float temp, float hum, int photocellReading, String d);
String getFormatFullDate(DateTime date);
String getFormatShortDate(DateTime date);
int leerLuminosidad();



void setup(){ 
    Serial.begin(9600);

    lcd.begin(16, 2);
    rtc.begin();

    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
    
    if (rtc.lostPower()){
      Serial.print("AJUSTANDO LA FECHA\n");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    
}

void loop(){ 
  int err;
  float temp, hum;
  int photocellReading = leerLuminosidad(); 
  
  DateTime now = rtc.now();

    if((err = dht11.read(hum, temp)) == 0)    // Si devuelve 0 es que ha leido bien
    {
      mostrarLCD(temp, hum, photocellReading, getFormatShortDate(now));
      String c = Serial.readString();
      Serial.println();
      if(c[0] == '[' && c[2]==']'){
          if(c[1]=='T')
          {
              Serial.print("[T: "); Serial.print(temp);  Serial.println("]");
          }
          else if(c[1]=='H')
          {
            Serial.print("[H: "); Serial.print(hum);  Serial.println("]");
          }
          else if(c[1]=='L')
          {
            Serial.print("[L: "); Serial.print(photocellReading);  Serial.println("]");
          }
          else if(c[1]=='D')
          {
            Serial.print("[D: "); Serial.print(getFormatFullDate(now)); Serial.println("]");
          }
        }
 }else{
    Serial.print("Error Num :");
    Serial.print(err);
    lcd.print("Error Num : ");
    lcd.print(err);
  }
  
  //delay(1000);            //Recordad que solo lee una vez por segundo
  
}


String getFormatFullDate(DateTime date)
{
   char s[100];
   sprintf(s, "%d-%d-%d %d:%d", date.year(),  date.month(), date.day(), date.hour(), date.minute());
   return s;

}

String getFormatShortDate(DateTime date)
{
   char s[100];
   sprintf(s, "%d-%d %d:%d",  date.month(), date.day(), date.hour(), date.minute());
   return s;

}

int leerLuminosidad()
{
  return analogRead(pin_photocell);
}

void mostrarSerial(float temp, float hum, int photocellReading, String d)
{
    Serial.print("T: ");
    Serial.print(temp);
    Serial.println();
    Serial.print("H: ");
    Serial.print(hum);
    Serial.println();
    Serial.print("L: ");
    Serial.print(photocellReading);
    Serial.println();
    
    Serial.print("D: ");
    Serial.print(d);
    Serial.println();
}

void mostrarLCD(float temp, float hum, int photocellReading, String d)
{
    lcd.print("T ");
    lcd.print(temp);
    lcd.print(" H ");
    lcd.print(hum);
    lcd.setCursor(0, 1);
    lcd.print("L ");
    lcd.print(photocellReading);
    lcd.print(" ");
    lcd.print(d);
}

