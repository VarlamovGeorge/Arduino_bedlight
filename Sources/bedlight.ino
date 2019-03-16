//Пин, отвечающий за прием данных об освещенности:
#define DARKNESS_PIN A5
//Пин, отвечающий за прием данных о пороге освещенности:
#define THRESHOLD_PIN A1
//Пин, отвечающий за включение
#define LEDSTRIP_PIN 13
//Пины, к которым подключены PIR-сенсоры (датчики движения HC-SR501)
#define PIR1 10
#define PIR2 11
#define PIR3 12

//Пин, отвечающий за прерывание по ручной кнопке (включение/выключение подсветки)
#define interruptPin 2

//Длительность работы подсветки после включения (сек.):
int lightDur=15;

//Величина освещенности:
int sensorValue=0;
//Статус подсветки (изначально выключена):
boolean ledStatus=false;

void setup() {
  Serial.begin(9600);
  pinMode(LEDSTRIP_PIN, OUTPUT);
  digitalWrite(LEDSTRIP_PIN, LOW); //Выключаем подсветку при запуске программы
  //Инициализируем входы сенсоров присутствия:
  pinMode(PIR1, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(PIR3, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), manualSwitch, FALLING);
}


void loop() {
  sensorValue = analogRead(DARKNESS_PIN);//Считываем текущую освещенность
  int thresholdValue = analogRead(THRESHOLD_PIN);//Считываем порог освещенности с потенциометра
  Serial.println("sensorValue="+String(sensorValue)+" Threshold value="+String(thresholdValue));
  
  while(sensorValue>thresholdValue){//Если достаточно темно
  boolean humanDetected = digitalRead(PIR1) | digitalRead(PIR2) | digitalRead(PIR3);
  Serial.println("Human detected? "+String(humanDetected));
    if(!ledStatus && humanDetected){//Если подсветка была выключена и сработал один из сенсоров, то включаем подсветку на X секунд
      ledstripOn();//Включаем подсветку
      delay(1000*lightDur);// Ждем заданное в lightDur время в секундах
      ledstripOff();//Выключаем подсветку
    }
    sensorValue = analogRead(DARKNESS_PIN);//Считываем текущую освещенность
  }
}

void ledstripOn(){
  digitalWrite(LEDSTRIP_PIN, HIGH);
  Serial.println("Light is ON for "+String(lightDur)+" seconds. Lightness value="+String(sensorValue));
  ledStatus=true;
}

void ledstripOff(){
  digitalWrite(LEDSTRIP_PIN, LOW);
  Serial.println("Light is OFF. Lightness value="+String(sensorValue));
  ledStatus=false;
}

void manualSwitch(){
  Serial.println("EXEPTION!");
  if(ledStatus){
    ledstripOff();
  }
  else{
    ledstripOn();
  }
  delay(100);
}
