#define BUTTON_PIN D5
void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(BUTTON_PIN))
  {
    Serial.println("Low");
  }
  else
  {
    Serial.println("High");
  }
  delay(200);
}
