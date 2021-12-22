#include <Stepper.h>
#include "SevSeg.h"

#define led1Red 43
#define led1Green 45
#define led2Red 47
#define led2Green 49
#define led3Red 51
#define led3Green 53

const int stepsPerPart = 1625;
const int portSteps = 1700;
long imageIndex1;
long imageIndex2;
long imageIndex3;

long oldImageIndex1 = 2;
long oldImageIndex2 = 2;
long oldImageIndex3 = 2;
String gameState = String("configuring");

const int lightSensorC3C1 = A1; //card x character y
const int lightSensorC3C2 = A2;
const int lightSensorC3C3 = A3;

const int lightSensorC2C1 = A4;
const int lightSensorC2C2 = A5;
const int lightSensorC2C3 = A6;

const int lightSensorC1C1 = A7;
const int lightSensorC1C2 = A8;
const int lightSensorC1C3 = A9;

int inactive = 0;
int inactiveMax = 20000;
bool resetted = false;

int score = 0;

String character1;
String character2;
String character3;


const int stepsPerRevolution = 512;

SevSeg sevseg; //Instantiate a seven segment object

Stepper windowMotor1(stepsPerRevolution, 2, 4, 3, 5);
Stepper windowMotor2(stepsPerRevolution, 6, 8, 7, 9);
Stepper windowMotor3(stepsPerRevolution, 10, 12, 11, 13);
Stepper portMotor(stepsPerRevolution, 14, 16, 15, 17);

void setup() {
  Serial.begin(9600);
  windowMotor1.setSpeed(60);
  windowMotor2.setSpeed(60);
  windowMotor3.setSpeed(60);
  portMotor.setSpeed(60);
  randomSeed(analogRead(0));

  pinMode(lightSensorC1C1, INPUT);
  pinMode(lightSensorC1C2, INPUT);
  pinMode(lightSensorC1C3, INPUT);
  pinMode(lightSensorC2C1, INPUT);
  pinMode(lightSensorC2C2, INPUT);
  pinMode(lightSensorC2C3, INPUT);
  pinMode(lightSensorC3C1, INPUT);
  pinMode(lightSensorC3C2, INPUT);
  pinMode(lightSensorC3C3, INPUT);

  pinMode(led1Red, OUTPUT);
  pinMode(led1Green, OUTPUT);
  pinMode(led2Red, OUTPUT);
  pinMode(led2Green, OUTPUT);
  pinMode(led3Red, OUTPUT);
  pinMode(led3Green, OUTPUT);
  

  byte numDigits = 4;
  byte digitPins[] = {22, 24, 26, 28};
  byte segmentPins[] = {30, 32, 34, 36, 38, 40, 42};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setNumber(score);
  sevseg.refreshDisplay();
}

void loop() {
//Serial.println(inactive);

  
  if(gameState.equals("configuring")){
    //portMotor.step(-portSteps);

  for(int i=0; i < portSteps; i++){
    portMotor.step(1);
    sevseg.refreshDisplay();
  }
     
    imageIndex1 = random(1,4);
    if(imageIndex1 == 3){
      character1 = "humanAlive";
    }else{
      character1 = "monsterAlive";
    }

    imageIndex2 = random(1,4);
    if(imageIndex2 == 3){
      character2 = "humanAlive";
    }else{
      character2 = "monsterAlive";
    }

    imageIndex3 = random(1,4);
    if(imageIndex3 == 3){
      character3 = "humanAlive";
    }else{
      character3 = "monsterAlive";
    }

    if(imageIndex1 == 3 && imageIndex2 == 3 && imageIndex3 == 3){ // zodat er geen 3 mensen zijn
        imageIndex3 = random(1,3);
        character3 = "monsterAlive";
    }
    
    slideCards();
    
    for(int i=0; i < portSteps; i++){
      portMotor.step(-1);
      sevseg.refreshDisplay();
    }
    
    gameState = "shooting";
    Serial.println("shoot!");
  }
  
  if(gameState.equals("shooting")){
    int lightSensorC1C1Status = analogRead(lightSensorC1C1);
    int lightSensorC1C2Status = analogRead(lightSensorC1C2);
    int lightSensorC1C3Status = analogRead(lightSensorC1C3);
    int lightSensorC2C1Status = analogRead(lightSensorC2C1);
    int lightSensorC2C2Status = analogRead(lightSensorC2C2);
    int lightSensorC2C3Status = analogRead(lightSensorC2C3);
    int lightSensorC3C1Status = analogRead(lightSensorC3C1);
    int lightSensorC3C2Status = analogRead(lightSensorC3C2);
    int lightSensorC3C3Status = analogRead(lightSensorC3C3);
    //Serial.println(lightSensorC3C2Status);

    int ldrTrigger = 800;

    if(lightSensorC1C1Status > ldrTrigger || lightSensorC1C2Status > ldrTrigger) {
      inactive = 0;
      resetted = false;
      character1 = "monsterDead";
      digitalWrite(led1Green, HIGH);
    }
    
    if(lightSensorC1C3Status > ldrTrigger) {
      inactive = 0;
      resetted = false;
      character1 = "humanDead";
      digitalWrite(led1Red, HIGH);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led1Red, LOW);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led1Red, HIGH);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led1Red, LOW);
      resetGame();
    }

    if(lightSensorC2C1Status > ldrTrigger || lightSensorC2C2Status > ldrTrigger) {
      inactive = 0;
      resetted = false;
      character2 = "monsterDead";
      digitalWrite(led2Green, HIGH);
    }
    
    if(lightSensorC2C3Status > ldrTrigger) {
      inactive = 0;
      resetted = false;
      character2 = "humanDead";
      digitalWrite(led2Red, HIGH);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led2Red, LOW);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led2Red, HIGH);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led2Red, LOW);
      resetGame();
    }

    if(lightSensorC3C1Status > ldrTrigger || lightSensorC3C2Status > ldrTrigger) {
      inactive = 0;
      resetted = false;
      character3 = "monsterDead";
      digitalWrite(led3Green, HIGH);
    }
    
    if(lightSensorC3C3Status > ldrTrigger) {
      inactive = 0;
      resetted = false;
      character3 = "humanDead";
      digitalWrite(led3Red, HIGH);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led3Red, LOW);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led3Red, HIGH);
      for(int i = 0; i < 500; i++){
        delay(1);
        sevseg.refreshDisplay();
      }
      digitalWrite(led3Red, LOW);
      resetGame();
    }

    if(character1 == "monsterDead" || character1 == "humanAlive"){
      if(character2 == "monsterDead" || character2 == "humanAlive"){
        if(character3 == "monsterDead" || character3 == "humanAlive"){
            digitalWrite(led1Green, HIGH);
            digitalWrite(led2Green, HIGH);
            digitalWrite(led3Green, HIGH);
            for(int i = 0; i < 500; i++){
              delay(1);
              sevseg.refreshDisplay();
            }
            digitalWrite(led1Green, LOW);
            digitalWrite(led2Green, LOW);
            digitalWrite(led3Green, LOW);
            for(int i = 0; i < 500; i++){
              delay(1);
              sevseg.refreshDisplay();
            }
            digitalWrite(led1Green, HIGH);
            digitalWrite(led2Green, HIGH);
            digitalWrite(led3Green, HIGH);
            for(int i = 0; i < 500; i++){
              delay(1);
              sevseg.refreshDisplay();
            }
            digitalWrite(led1Green, LOW);
            digitalWrite(led2Green, LOW);
            digitalWrite(led3Green, LOW);
            
            gameState = "configuring";
            oldImageIndex1 = imageIndex1;
            oldImageIndex2 = imageIndex2;
            oldImageIndex3 = imageIndex3;
            score++;
            sevseg.setNumber(score);
        }
      }
    }
  }
  
  if(inactive < inactiveMax){
      inactive++;
  }

  if(inactive == inactiveMax && !resetted){
    resetGame();
    resetted = true;
  }

    /*Serial.print(character1);
    Serial.print(imageIndex1);
    Serial.print(", ");
    Serial.print(character2);
    Serial.print(imageIndex2);
    Serial.print(", ");
    Serial.print(character3);
    Serial.println(imageIndex3);*/
    sevseg.refreshDisplay();
}

void slideCards() {
  //windowMotor1.step((imageIndex1-oldImageIndex1)*stepsPerPart);

  for(int i=0; i < abs((imageIndex1-oldImageIndex1)*stepsPerPart); i++){
    
    if((imageIndex1-oldImageIndex1)*stepsPerPart > 0){
      windowMotor1.step(-1);
      delay(2);
    }else{
      windowMotor1.step(1);
      delay(2);
    }
    
    sevseg.refreshDisplay();
  }

  //windowMotor2.step((imageIndex2-oldImageIndex2)*stepsPerPart);

  for(int i=0; i < abs((imageIndex2-oldImageIndex2)*stepsPerPart); i++){
    
    if((imageIndex2-oldImageIndex2)*stepsPerPart > 0){
      windowMotor2.step(1);
      delay(2);
    }else{
      windowMotor2.step(-1);
      delay(2);
    }
    
    sevseg.refreshDisplay();
  }

  
  //windowMotor3.step((imageIndex3-oldImageIndex3)*stepsPerPart);

  for(int i=0; i < abs((imageIndex3-oldImageIndex3)*stepsPerPart); i++){
    
    if((imageIndex3-oldImageIndex3)*stepsPerPart > 0){
      windowMotor3.step(1);
      delay(2);
    }else{
      windowMotor3.step(-1);
      delay(2);
    }
    
    sevseg.refreshDisplay();
  }
}

void resetGame(){
  Serial.print("resetting");
  digitalWrite(led1Green, LOW);
  digitalWrite(led2Green, LOW);
  digitalWrite(led3Green, LOW);

  if(imageIndex1 == 3){
    character1 = "humanAlive";
  }else{
    character1 = "monsterAlive";
  }

  if(imageIndex2 == 3){
    character2 = "humanAlive";
  }else{
    character2 = "monsterAlive";
  }

  if(imageIndex3 == 3){
    character3 = "humanAlive";
  }else{
    character3 = "monsterAlive";
  }
  score = 0;
  sevseg.setNumber(score);
}
