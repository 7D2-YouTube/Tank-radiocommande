#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include "pitches.h"

RF24 radio(9, 10);

byte addresses[][6] = {"Addr1", "Addr2"};

#define ENABLE1 5
#define ENABLE2 6
#define DIRA 3
#define DIRB 4
#define DIRC 7
#define DIRD 8
#define BUZZER 2

typedef struct {
  int j1 =0;
  int j2 = 0;
  int sw = 1;
}data;
data payload;

void setup() {
  Serial.begin(9600);
  pinMode(ENABLE1, OUTPUT);
  pinMode(ENABLE2, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(DIRC, OUTPUT);
  pinMode(DIRD, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  //Initialiser le module radio
  radio.begin();

  //Puissance de transmission
  radio.setPALevel(RF24_PA_HIGH);

  //Vitesse de transmission
  radio.setDataRate(RF24_2MBPS);

  //Définition du channel
  radio.setChannel(124);
  
  //Ouvrir un channel d'écriture et un channel de lecture
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  //On commence à écouter le module radio
  radio.startListening();
}

void loop(){
  if(radio.available()){

    while(radio.available()){
      radio.read(&payload, sizeof(payload));
    }

    //On arrête d'écouter le module radio
    radio.stopListening();
    
    //Puis on écrit dans la variable
    radio.write( &payload, sizeof(payload) );

    //On recommence a écouter
    radio.startListening();

    Serial.print("Joystick1 ");
    Serial.println(payload.j1);
    Serial.print("Joystick2 ");
    Serial.println(payload.j2);
    Serial.print("Switch ");
    Serial.println(payload.sw);
  }
  
  //Contrôle du moteur de gauche
  if(payload.j1 > 600) {
    payload.j1 = map(payload.j1, 600, 0, 0, 255);
    digitalWrite(ENABLE1, HIGH);
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
  }

  else if(payload.j1 < 400) {
    payload.j1 = map(payload.j1, 400, 0, 0, 255);
    digitalWrite(ENABLE1, HIGH);
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);
  }

  else {
    digitalWrite(ENABLE1, LOW);
  }

  //Contrôle du moteur de droite
  if(payload.j2 > 600) {
    payload.j2 = map(payload.j2, 600, 0, 0, 255);
    digitalWrite(ENABLE2, HIGH);
    digitalWrite(DIRC, HIGH);
    digitalWrite(DIRD, LOW);
  }

  else if(payload.j2 < 400) {
    payload.j2 = map(payload.j2, 400, 0, 0, 255);
    digitalWrite(ENABLE2, HIGH);
    digitalWrite(DIRC, LOW);
    digitalWrite(DIRD, HIGH);
  }

  else {
    digitalWrite(ENABLE2, LOW);
  }

  if(payload.sw == 0){
    tone(BUZZER, NOTE_C5, 100);
  }

  delay(100);
}
