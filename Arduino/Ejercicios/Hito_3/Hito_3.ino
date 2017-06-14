// DHT sensor library - Version: Latest 

// http://hetpro-store.com/TUTORIALES/mcp9700-sensor-de-temperatura/


void setup(){ 
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(11, OUTPUT);
}

void loop(){ 
  digitalWrite(13, HIGH);
  analogWrite(11, 255);

  delay(3000);

  analogWrite(11, 0);

  delay(3000);
  
}
