/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A2);
  // print out the value you read:
  Serial.println(sensorValue);
  if(sensorValue>200){
    digitalWrite(13, HIGH);
  }
  else{digitalWrite(13, LOW);}
  delay(1000);        // delay in between reads for stability
}
