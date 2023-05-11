#include <HCSR04.h>
#include <Servo.h>
#include <Wire.h>

//definirea servomotorului:
Servo servo;

//definirea celor doi senzori:
UltraSonicDistanceSensor sensorIn(A2, A3);
UltraSonicDistanceSensor sensorOut(A0, A1);

//definirea valorilor citite de la senzori:
int sensorInValue;
int sensorOutValue;

//definirea numarului de locuri din parcare:
int freePlaces = 9;

//definirea flagurilor:
int isIn = 0;
int isOut = 0;

//definirea LED-urilor:
int redLed = 5;
int greenLed = 6;

//functie pentru aprinderea ledurilor
void ledHandler() {
  if (freePlaces > 0) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
  } else if (freePlaces == 0) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
  }
}

//functie pentru intrarea in parcare
void enterHandler(int sensorInValue, int sensorOutValue) {
  if ((sensorInValue >= 2 && sensorInValue <= 5) && freePlaces > 0 && isIn == 0) {
    for (int p=80 ; p>=0 ; p=p-4) {
      printf("Bariera se deschide pentru intrare!");
    } 
    isIn = 1;
    freePlaces--;
  }
  if ((sensorOutValue >= 7 && sensorOutValue <= 9)) {
    delay(2000);
    for (int p=0 ; p<=80; p=p+4) {
      printf("Bariera se inchide pentru intrare!");
    }
    isIn = 0;
  }
}

//functie pentru iesirea din parcare
void exitHandler(int sensorInValue, int sensorOutValue) {
  if ((sensorOutValue >= 2 && sensorOutValue <= 5) && isOut == 0) {
    for (int p=95 ; p>=10 ; p=p-4) {
      printf("Bariera se deschide pentru iesire!");
    }
    isOut = 1;
    freePlaces++;
  }

  if (sensorInValue >= 7 && sensorInValue <=9) {
    delay(2000);
    for (int p=10 ; p<=95 ; p=p+2) {
      printf("Bariera se inchide pentru iesire!");
    }
    isOut = 0;
  }
}

//functie pentru verificarea locurilor libere;
void verifyFreePlaces() {
  if (freePlaces >= 9) {
      freePlaces = 9;
  }
  if (freePlaces < 0) {
      freePlaces  = 0;
  }
}




void setup() {
  Serial.begin(115200);

  //PIN-ul 9 va fi responsabil de controlul servomotorului:
  servo.attach(9);

  //setarea pozitiei initiale a barierei:
  servo.write(80);


  //setarea LED-urilor ca output:
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

}

void loop() {
  //atribuim variabilelor de masurare valorile masurate de senzori.
  sensorInValue = sensorIn.measureDistanceCm();
  sensorOutValue = sensorOut.measureDistanceCm();

  //aprindere leduri:
  ledHandler();

  //intrare in parcare:
  enterHandler(sensorInValue, sensorOutValue);

  //iesire din parcare:
  exitHandler(sensorInValue, sensorOutValue);

  //verificare interval (0, 10):
  verifyFreePlaces();
  
  delay(100);

}
