#include "HUSKYLENS.h"
#include "SoftwareSerial.h"


HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX


//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
int printResult(HUSKYLENSResult result);

bool rightobj = false;
int command;
int left = 4;
int forward = 3;
int right = 5;

int oSize = 70;

int xMin = 220;
int xMax = 205;

void setup() {

  pinMode(forward, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);

  Serial.begin(115200);
  mySerial.begin(9600);

  while (!huskylens.begin(mySerial))
  {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
}

void loop() {

  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else
  {
    Serial.println(F("###########"));
    Serial.println(huskylens.available());

    if (huskylens.available())
    {
      rightobj = false;
      while (huskylens.available())
      {
          HUSKYLENSResult result = huskylens.read();
          command = printResult(result);
      }
      Serial.print(command);
      if (rightobj == true) {
        switch (command) {
          case 0: goLeft();
            break;
          case 1: goPick();
            break;
          case 2: goRight();
            break;
          case 3: goForward();
            break;
          default: noGo();
        }
      }
       else{
         noGo();
       }
      
    }
    else
    {
      noGo();
    }

  }
  delay(300);

}

// Defining Signal
void goLeft()
{
  Serial.println("Turn left!");
  digitalWrite(left, HIGH);
  digitalWrite(forward, LOW);
  digitalWrite(right, LOW);
}

void goForward()
{
  Serial.println("Go forward!");
  digitalWrite(forward, HIGH);
  digitalWrite(left, LOW);
  digitalWrite(right, LOW);
}

void goRight()
{
  Serial.println("Turn right!");
  digitalWrite(right, HIGH);
  digitalWrite(left, LOW);
  digitalWrite(forward, LOW);
}

void noGo()
{
  Serial.println("NOGO!");
  digitalWrite(left, LOW);
  digitalWrite(forward, LOW);
  digitalWrite(right, LOW);
}

void goPick()
{
  Serial.println("Pick up object!");
  digitalWrite(left, HIGH);
  digitalWrite(forward, LOW);
  digitalWrite(right, HIGH);
  //delay(2000);
}

int printResult(HUSKYLENSResult result) {


  //Serial.println(result.height);
  //Serial.println(result.width);

  if ( (result.width < result.height/1.5) && (result.ID == 1)) // Conditions for right dimension of object.
  {
    rightobj = true;
    if (result.command == COMMAND_RETURN_BLOCK)
    {
      Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
      // Any ID above the number 0 is learned. Non-learned object are all 0.
      Serial.println(result.ID);


      if ( (result.xCenter >= (xMin - (oSize - result.width / 2))) && (result.xCenter <= xMax + (oSize - result.width / 2)) && ( result.yCenter <= 145 && result.yCenter > 110 ) && result.height >= 160)
      {

        return 1;


      }


      else if (result.xCenter <= xMin - (oSize - result.width / 2))
      {
        return 0;
      }


      else if (result.xCenter >= xMax + (oSize - result.width / 2))
      {
        return 2;
      }
      else if (((result.xCenter >= xMin - (oSize - result.width / 2)) && (result.xCenter <= xMax + (oSize - result.width / 2))) || result.yCenter < 100)
      {
        return 3;
      }
    }
  }

  else
  {
    Serial.println("Object unknown!");
    if (rightobj == true) return command;
    else return -1;
  }

  

}
