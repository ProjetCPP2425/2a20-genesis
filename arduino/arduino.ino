#include <Servo.h>

Servo monServo;  // Créer un objet Servo

void setup() {
  monServo.attach(9);  // Attache le servo au pin D9
}

void loop() {
  monServo.write(0);   // Aller à 0 degrés
  delay(1000);         // Attendre 1 seconde
  monServo.write(90);  // Aller à 90 degrés
  delay(1000);         // Attendre 1 seconde
  monServo.write(180); // Aller à 180 degrés
  delay(1000);         // Attendre 1 seconde
}

