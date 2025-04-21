#define LED_PIN D6
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(LED_PIN))
  {
    Serial.println("Low");
  }
  else
  {
    Serial.println("High");
  }
  delay(200);
}
