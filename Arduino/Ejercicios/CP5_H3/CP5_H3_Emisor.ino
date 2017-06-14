// DHT sensor library - Version: Latest 

// http://hetpro-store.com/TUTORIALES/mcp9700-sensor-de-temperatura/

#include <DHT11.h>

#include "RTClib.h"

/* Input-side (button) Arduino code */
#include "SoftwareSerial.h"

// Pin A4: SDA, A5: SCL
// Pin A2: Luminosidad

int pin_dht11 = 2;
int pin_photocell = A2;
int pin_RX = 3;
int pin_TX = 4;

// RX: Arduino pin 2, XBee pin DOUT.  TX:  Arduino pin 3, XBee pin DIN
SoftwareSerial XBee(pin_RX, pin_TX);
DHT11 dht11(pin_dht11);
RTC_DS3231 rtc;

void setup(){ 
    Serial.begin(9600);

    // Baud rate MUST match XBee settings (as set in XCTU)
    XBee.begin(9600);

    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
    
    if (rtc.lostPower()){
      Serial.print("AJUSTANDO LA FECHA\n");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
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

void loop(){ 
  int err;
  float temp, hum;
  Serial.print("Inicio del programa\n");
  int photocellReading = leerLuminosidad(); 
  
  DateTime now = rtc.now();
  
  if((err = dht11.read(hum, temp)) == 0)    // Si devuelve 0 es que ha leido bien
  {
    
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.print("\nHumedad: ");
    Serial.print(hum);
    Serial.print("\nLuminosidad: ");
    Serial.print(photocellReading);
    Serial.println();
    
    Serial.print(getFormatFullDate(now));
    Serial.println();

    if(photocellReading < 500){
      XBee.write('l');
    }

    if(temp > 28){
      XBee.write('m');
    }
    
  }else{
    Serial.print("Error Num :");
    Serial.print(err);
  }
  
  delay(1000);            //Recordad que solo lee una vez por segundo
  
}
