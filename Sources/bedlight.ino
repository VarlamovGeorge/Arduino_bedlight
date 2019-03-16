//Пин, отвечающий за прием данных об освещенности:
#define DARKNESS_PIN A2
//Пин, отвечающий за включение
#define LEDSTRIP_PIN 13

//Величина освещенности:
int sensorValue=0;
boolean ledStatus=false;

void setup() {
  Serial.begin(9600);
  pinMode(LEDSTRIP_PIN, OUTPUT);
  digitalWrite(LEDSTRIP_PIN, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  sensorValue = analogRead(DARKNESS_PIN);//Считываем текущую освещенность
  //Serial.println("sensorValue="+String(sensorValue));
  if(sensorValue>200 & !ledStatus){
    digitalWrite(LEDSTRIP_PIN, HIGH);
    Serial.println("Light is ON. Lightness value="+String(sensorValue));
    ledStatus=true;
  }
  else if(sensorValue<=200 & ledStatus){
    digitalWrite(LEDSTRIP_PIN, LOW);
    Serial.println("Light is OFF. Lightness value="+String(sensorValue));
    ledStatus=false;
  }
  delay(500);        // delay in between reads for stability
}
