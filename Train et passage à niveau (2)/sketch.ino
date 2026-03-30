#include <Servo.h>

const int leds[] = {2, 3, 4, 5};

// Boutons
const int entreeGD = 13;
const int sortieGD = 12;
const int entreeDG = 10;
const int sortieDG = 9;

const int boutons[] = {entreeGD, sortieGD, entreeDG, sortieDG};

Servo servoGauche;
Servo servoDroit;

int nbTrains = 0; // compteur de trains présents

// Anti-rebond simple (mémoire des anciens états)
bool etatAvant[4] = {HIGH, HIGH, HIGH, HIGH};

void setup() {

  // LEDs
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Boutons
  for (int i = 0; i < 4; i++) {
    pinMode(boutons[i], INPUT_PULLUP);
  }

  // Servos
  servoGauche.attach(6);
  servoDroit.attach(7);

  leverBarrieres();
}

void loop() {

  // Lecture boutons avec détection de front (appui unique)
  for (int i = 0; i < 4; i++) {

    bool etatActuel = digitalRead(boutons[i]);

    // Détection appui (passage HIGH -> LOW)
    if (etatAvant[i] == HIGH && etatActuel == LOW) {

      // Entrées trains
      if (boutons[i] == entreeGD || boutons[i] == entreeDG) {
        nbTrains++;
      }

      // Sorties trains
      if (boutons[i] == sortieGD || boutons[i] == sortieDG) {
        if (nbTrains > 0) nbTrains--;
      }
    }

    etatAvant[i] = etatActuel;
  }

  // Gestion sécurité : passage fermé tant qu'il reste un train
  if (nbTrains > 0) {

    baisserBarrieres();

    // Clignotement alterné
    for (int i = 0; i < 4; i++) {
      digitalWrite(leds[i], (i % 2 == 0) ? HIGH : LOW);
    }
    delay(500);

    for (int i = 0; i < 4; i++) {
      digitalWrite(leds[i], (i % 2 == 1) ? HIGH : LOW);
    }
    delay(500);

  } else {

    leverBarrieres();

    // Éteindre LEDs
    for (int i = 0; i < 4; i++) {
      digitalWrite(leds[i], LOW);
    }
  }
}

void baisserBarrieres() {
  servoGauche.write(90);
  servoDroit.write(90);
}

void leverBarrieres() {
  servoGauche.write(0);
  servoDroit.write(0);
}