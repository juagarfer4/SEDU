// DHT sensor library - Version: Latest 

// http://hetpro-store.com/TUTORIALES/mcp9700-sensor-de-temperatura/

#include <DHT11.h>

#include <LiquidCrystal.h>

#include <RTClib.h>

DHT11 dht11(2);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
RTC_DS3231 rtc;

void setup(){ 
    Serial.begin(9600);
    lcd.begin(16, 2);
    rtc.begin();

    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
    
    if (rtc.lostPower()){
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

void loop(){ 
  int err;
  float temp, hum;
  Serial.print("Inicio del programa\n");
  lcd.setCursor(0, 0);
  int photocellReading = analogRead(A2); 
  
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
    lcd.print("T ");
    lcd.print(temp);
    lcd.print(" H ");
    lcd.print(hum);
    lcd.setCursor(0, 1);
    lcd.print("L ");
    lcd.print(photocellReading);
    lcd.print(" ");
    lcd.print(getFormatShortDate(now));
    
  }else{
    Serial.print("Error Num :");
    Serial.print(err);
    lcd.print("Error Num : ");
    lcd.print(err);
  }
  delay(1000);            //Recordad que solo lee una vez por segundo
  
}
