#include "Pitches.h"

const int NUMBER_OF_COLUMNS = 3;
const int NUMBER_OF_ROWS = 8;
const int NUMBER_OF_BUTTONS = 3;
const int pinForCol[3] = {11, 12, 13};
const int pinForRow[8] = {10, 9, 8, 7, 6, 5, 4, 3};
const int speakerPin = 15;

volatile boolean missileLaunched = false;

int gameSpeed = 1000; //300 is the minimum value the arduino can handle
int loopCycles = 0;
int bombRowForColumn[3] = {8, 8, 8};//8 is a flag value which means there is no bomb in the column
int missileRowForColumn[3] = {-1, -1, -1};//-1 is a flag value which means there is no missile in the column
int buttonInput = 0;
int score = 0;
int columnsDestroyed = 0;

boolean ledValues[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {
{false, false, false},
{false, false, false},
{false, false, false},
{false, false, false},
{false, false, false},
{false, false, false},
{false, false, false},
{false, false, false},
};

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));// pin0 is disconnected so noise generates random numbers
  for (int row = 0; row < NUMBER_OF_ROWS; row++){
  pinMode(pinForRow[row], OUTPUT);
  }
  for (int col = 0; col < NUMBER_OF_COLUMNS; col++){
  pinMode(pinForCol[col], OUTPUT);
  }
  pinMode(speakerPin, OUTPUT);
  attachInterrupt(0, missileFired, RISING);
}

void loop() {
  if(loopCycles == gameSpeed){//positions of objects are only updated when loopCycles = gameSpeed
    loopCycles = 0;
    
    //handles generation of bombs
    int newBombColumn = random(3); //generates column between 0 and 2
    if (bombRowForColumn[newBombColumn] == 8){ //if 8, then a bomb does not exist in the column
      bombRowForColumn[newBombColumn] = -1; //generates bomb in row -1
    }

    //handles generation of missiles
    if (missileLaunched){
      buttonInput = analogRead(A0);
      if (buttonInput > 1019 && buttonInput < 1024 && missileRowForColumn[0] == -1 && missileRowForColumn[0] != 9){
        missileRowForColumn[0] = 8;//blue column
      } else if (buttonInput > 850 && buttonInput < 855 && missileRowForColumn[1] == -1 && missileRowForColumn[1] != 9){
        missileRowForColumn[1] = 8;//red column
      } else if (buttonInput > 678 && buttonInput < 683 && missileRowForColumn[2] == -1 && missileRowForColumn[2] != 9){
        missileRowForColumn[2] = 8;//white column
      } else {
//        Serial.println("Button error");
      }
      missileLaunched = false;
      tone(speakerPin, NOTE_E7, 40);
    }
    
    //handles motion of objects
    for (int col = 0; col < NUMBER_OF_COLUMNS; col++){
      //handles bombs falling
      int bombRow = bombRowForColumn[col];
      if (bombRow < 7){ //if bombRow == 8 then there is no bomb in the column
          bombRowForColumn[col]++;//bomb falls one row, when bomb reaches row 8 it is no longer in play
          ledValues[bombRowForColumn[col]][col] = true; //turns on led in bomb's current position
          ledValues[bombRowForColumn[col]-1][col] = false;//turns off led in bomb's previous position
      } else if (bombRow == 7){
          for (int row = 0; row < NUMBER_OF_ROWS; row++){
            ledValues[row][col] = true;
          }
          missileRowForColumn[col] = 9;
          columnsDestroyed++;
          tone(speakerPin, NOTE_E1, 1000);
      }
      
      
      //handles missiles rising
      int missileRow = missileRowForColumn[col];
      if (missileRow > -1 && missileRow < 9){ //if missileRow == -1 then there is no missile in the column
          missileRowForColumn[col]--;//missile rises one row, when missile reaches row -1 it is no longer in play
          ledValues[missileRowForColumn[col]][col] = true; //turns on led in missile's current position
          ledValues[missileRowForColumn[col]+1][col] = false;//turns off led in missile's previous position
      }

      //handles missile/bomb collision
      int distanceBetweenMissileAndBomb = missileRowForColumn[col] - bombRowForColumn[col];
      if (distanceBetweenMissileAndBomb == 0 || distanceBetweenMissileAndBomb == 1){
        ledValues[bombRowForColumn[col]][col] = false;//turn off LED for bomb
        ledValues[missileRowForColumn[col]][col] = false;//turn off LED for missile
        bombRowForColumn[col] = 8; //when bomb reaches row 8 it is no longer in play
        missileRowForColumn[col] = -1; //when missile reaches row -1 it is no longer in play
        tone(speakerPin, NOTE_E5, 50);
      }
    }
    if (gameSpeed > 300){
    gameSpeed -= 20;//game gets faster until gameSpeed reaches 400
    }
    if (columnsDestroyed < 3){
      score++;
    } else if (columnsDestroyed == 3){
      tone(speakerPin, NOTE_E1, 1000);
      Serial.print("Score: ");
      Serial.println(score);
      columnsDestroyed++;
    }
  }
  updateLEDs();
  loopCycles++;
}

void updateLEDs(){
  for (int col = 0; col < NUMBER_OF_COLUMNS; col++){
    digitalWrite(pinForCol[col], LOW);//disable current column
    
    for (int row = 0; row < NUMBER_OF_ROWS; row++){
      if (row-1 >= 0){
      digitalWrite(pinForRow[row-1], HIGH);//disable previous row
      }
      digitalWrite(pinForRow[row], HIGH);//disable row
      boolean ledIsOn = ledValues[row][col];
      
      if (ledIsOn == true){
        digitalWrite(pinForRow[row], LOW);//enable row
        digitalWrite(pinForCol[col], HIGH);//enable column
      
      }else if (ledIsOn == false) {
        digitalWrite(pinForRow[row], HIGH);//disable row
      }
    }
    for (int row = 0; row < NUMBER_OF_ROWS; row++){
      digitalWrite(pinForRow[row], HIGH);//disables all rows for the current column
    }
    digitalWrite(pinForCol[col], LOW);//disable current column
  }
}

void missileFired(){
  missileLaunched = true; //maybe put an anologRead here instead
//  buttonInput = analogRead(A0);
}
