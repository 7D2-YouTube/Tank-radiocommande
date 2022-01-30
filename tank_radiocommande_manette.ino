#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);

byte addresses[][6] = {"Addr1", "Addr2"};

#define joystick1 A0
#define joystick2 A1
#define Switch 2

int val1;
int val2;
int valS;

typedef struct {
  int j1 = 0;
  int j2 = 0;
  int sw = 1;
}data;
data payload;

void setup() {
  Serial.begin(9600);
  pinMode(joystick1, INPUT);
  pinMode(joystick2, INPUT);
  pinMode(Switch, INPUT);
  digitalWrite(Switch, HIGH);

  //Initialiser le module radio
  radio.begin();

  //Puissance de transmission
  radio.setPALevel(RF24_PA_HIGH);

  //Vitesse de transmission
  radio.setDataRate(RF24_2MBPS);

  //Définition du channel
  radio.setChannel(124);

  //Ouvrir un channel d'écriture et un channel de lecture
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  //On va envoyer des nombres aléatoires
  randomSeed(analogRead(A0));
  randomSeed(analogRead(A1));
}

void loop(){
  val1 = analogRead(joystick1);
  payload.j1 = val1;

  val2 = analogRead(joystick2);
  payload.j2 = val2;

  valS = digitalRead(Switch);
  payload.sw = valS;
  
  //On arrête d'écouter pour pouvoir transmettre
  radio.stopListening();

  radio.write(&payload, sizeof(payload));

  delay(10);
}
