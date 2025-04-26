#define BUZZER_PIN 8
char data;

void setup() {
  Serial.begin(9600); // Initialiser la communication série

  pinMode(BUZZER_PIN, OUTPUT); // Définir le buzzer comme sortie
  digitalWrite(BUZZER_PIN, LOW); // Éteindre le buzzer au départ
}


void loop() {
  if (Serial.available()) {
    data = Serial.read(); // Lire le caractère reçu

    if (data == '1') {
      digitalWrite(BUZZER_PIN, HIGH); // Allumer le buzzer
      Serial.write('1'); // Confirmer l'état ON
    } 
    else if (data == '0') {
      digitalWrite(BUZZER_PIN, LOW); // Éteindre le buzzer
      Serial.write('0'); // Confirmer l'état OFF
    }
  }
}
