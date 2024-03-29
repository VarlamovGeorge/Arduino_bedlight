//Пин, отвечающий за прием данных об освещенности:
#define DARKNESS_PIN A5
//Пин, отвечающий за прием данных о пороге освещенности:
#define THRESHOLD_PIN A1
//Пин, отвечающий за прием данных яркости освещения:
#define BRIGHT_PIN A2
//Пин, отвечающий за включение подсветки и управления ее якростью с помощью ШИМ
#define LEDSTRIP_PIN 3
//Пины, к которым подключены PIR-сенсоры (датчики движения HC-SR501)
#define PIR1 10
#define PIR2 11
#define PIR3 12

//Пин, отвечающий за прерывание по ручной кнопке (включение/выключение подсветки)
#define interruptPin 2

volatile unsigned long switch_time=0;
volatile unsigned long switch_time_PIR=0;

//Переменная для порога освещенности:
int thresholdValue;

//Длительность работы подсветки после включения от сенсора (сек.):
int lightDur=15;

//Длительность работы подсветки после включения от сенсора (мин.):
long int manLightDur=30;

//Величина освещенности:
int sensorValue=0;
//Величина яркость подсветки:
int brightnessValue=0;
//Статус подсветки (изначально выключена):
boolean ledStatus=false;

//Признак включения подсветки через PIR сенсор:
boolean PIRlight=false;

void setup() {
  Serial.begin(9600);
  pinMode(LEDSTRIP_PIN, OUTPUT);
  analogWrite(LEDSTRIP_PIN, 0); //Выключаем подсветку при запуске программы
  //digitalWrite(LEDSTRIP_PIN, LOW); //Выключаем подсветку при запуске программы
  //Инициализируем входы сенсоров присутствия:
  pinMode(PIR1, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(PIR3, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), manualSwitch, FALLING);
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  sensorValue = analogRead(DARKNESS_PIN);//Считываем текущую освещенность
  thresholdValue = analogRead(THRESHOLD_PIN);//Считываем порог освещенности с потенциометра
  brightnessValue = analogRead(BRIGHT_PIN);//Считываем требуемую яркость с потенциометра
  Serial.println("LED= "+String(ledStatus)+" SensorValue="+String(sensorValue)+" Threshold value="+String(thresholdValue)+" Brightness value="+String(brightnessValue)+" PIR detected="+String(PIRlight));
  Serial.println(millis()-switch_time);
  Serial.println(millis()-switch_time_PIR);
  Serial.println(1000L*60L*manLightDur);
  boolean humanDetected = digitalRead(PIR1) | digitalRead(PIR2) | digitalRead(PIR3);//Проверяем датчики движения
  
  if(ledStatus){
    analogWrite(LEDSTRIP_PIN, brightnessValue/4); //Включаем подсветку с помощью ШИМ
    if(PIRlight && (millis()-switch_time_PIR > 1000*lightDur)){//Подсветка горит и была включена с помощью PIR сенсора:
      if(humanDetected){//Время вышло, но снова обнаружено движение
        switch_time_PIR = millis();//Обновление счетчика времени
      }
      else{
        ledstripOff();//Выключаем подсветку при превышении таймером заданного промежутка времени lightDur (сек.)
      }
    }
    else if(!PIRlight && (millis()-switch_time > 1000L*60L*manLightDur)){//Подсветка горит и была включена с помощью кнопки:
      ledstripOff();//Выключаем подсветку  при превышения таймером заданного промежутка manLightDur (мин.)
    }
  }
  else{
    analogWrite(LEDSTRIP_PIN, 0); //Выключаем подсветку с помощью ШИМ
  }
  
  if(sensorValue>thresholdValue){//Если достаточно темно
    digitalWrite(LED_BUILTIN, HIGH);//Включаем светодиод на плате для индикации, что освещение теемнее, чем порог
    //boolean humanDetected = digitalRead(PIR1) | digitalRead(PIR2) | digitalRead(PIR3);
    Serial.println("Dark mode. PIR1= "+String(digitalRead(PIR1))+" PIR2= "+String(digitalRead(PIR2))+" PIR3= "+String(digitalRead(PIR3)));
    if(!ledStatus && humanDetected){//Если подсветка была выключена и сработал один из сенсоров, то включаем подсветку на X секунд
      PIRlight=true;//Выставляем признак включения подсветки от PIR сенсора
      switch_time_PIR = millis();
      ledstripOn();//Включаем подсветку
      //delay(1000*lightDur);// Ждем заданное в lightDur время в секундах
      //ledstripOff();//Выключаем подсветку
    }
    //sensorValue = analogRead(DARKNESS_PIN);//Считываем текущую освещенность
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);//Выключаем светодиод на плате
  }
}

void ledstripOn(){
  //analogWrite(LEDSTRIP_PIN, brightnessValue/4); //Включаем подсветку с помощью ШИМ
  //digitalWrite(LEDSTRIP_PIN, HIGH);
  //Serial.println("Light is ON for "+String(lightDur)+" seconds. Lightness value="+String(sensorValue)+ " Brightness="+String(brightnessValue/4));
  ledStatus=true;
}

void ledstripOff(){
  //analogWrite(LEDSTRIP_PIN, 0); //Выключаем подсветку с помощью ШИМ
  //digitalWrite(LEDSTRIP_PIN, LOW);
  Serial.println("Light is OFF. Lightness value="+String(sensorValue));
  ledStatus=false;
  PIRlight=false;//Сбрасываем признак включения через PIR
}

void manualSwitch(){//Обработчик прерываний по нажатию на кнопку ручного выключения/выключения подсветки
  if(millis()-switch_time>1000){//В целях защиты от дребезка меняем состояние не чаще, чем раз в 1000 мс
    if(ledStatus){
      ledstripOff();
    }
    else if(!ledStatus && sensorValue>thresholdValue){ //Кнопка на включение срабатывает только в темноте
      ledstripOn();
    }
  switch_time = millis();
  }
  
}
