int photoPin = A0;

void setup() {
  //start buzzer
  Serial.begin(9600);
  pinMode(11, OUTPUT);
}

void loop() {
  // photosensitive resistor, when the object reach, to get the reads less than 100, results the buzzer works 
  int light = analogRead(photoPin);
  Serial.println(light);
  delay(100);
  if(light<100)
  {
    //trigger the buzzer start rings two times
  digitalWrite(11, HIGH);
  delay(100);
  digitalWrite(11, LOW);
  delay(100);
   
  digitalWrite(11, HIGH);
  delay(100);
  digitalWrite(11, LOW);
  delay(100);
    
  }


}
