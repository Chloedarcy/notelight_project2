#include <Arduino.h>
#define LED 2

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("bello");
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}
