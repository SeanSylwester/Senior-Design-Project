#define trigPin 4
#define echoPin 5   
long timer = 0;

void setup() {
  Serial.begin (57600);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW); 
  pinMode(echoPin, INPUT); 
}

void loop() {
  long duration;
  uint16_t distance;

  timer = millis();
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1 * 10;			// distance calculation

  /*
  if(distance < 600 && distance > 10)
  {
    Serial.print("Time: "); Serial.print(millis() - timer); 
    Serial.print("ms - Distance: "); Serial.print(distance); Serial.println("cm");
  }
  */
  //Serial.println(distance);
  Serial.write(0x00FF & distance);
  Serial.write((0xFF00 & distance) >> 8);
  
  delay(250);
}
