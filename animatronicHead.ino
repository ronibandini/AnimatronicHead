// Simple Arduino Animatronic Head
// April 2021, Buenos Aires, Argentina
// Roni Bandini @RoniBandini

// Connect Servos to GND y VCC. Signals to D9, D8, D7
// Connect DfPlayer TX and RX to D11, D10. Busy to D6
// Upload audios to microSD 01/001.mp3 002.mp3, etc and edit howManyAudios

#include <Servo.h>
#include "SoftwareSerial.h"

// Mp3 player rx and tx
SoftwareSerial mySerial(10, 11);

Servo myservoEyeLeft;
Servo myservoEyeRight;
Servo myservoMouth;

# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00
# define ACTIVATED LOW

int pos = 0;
int pinEyeLeft=9;
int pinEyeRight=8;
int pinMouth=7;
int busyPin=6;
int isPlaying=HIGH;
int myMovement=0;
int myDelay=0;
int myRandom=0;
int howManyAudios=81+1;
int mouthOpen=0;

void setup() {

  Serial.begin(9600);
  Serial.println("Animatronic Head started...");
  Serial.println("Roni Bandini - Argentina 2021");

  randomSeed(analogRead(0));

  // mp3 player
  mySerial.begin(9600);
  pinMode(busyPin, INPUT);

  // Just init
  playFirst();

  // attach servos
  myservoEyeLeft.attach(pinEyeLeft);
  myservoEyeRight.attach(pinEyeRight);
  myservoMouth.attach(pinMouth);

  // Play first audio
  execute_CMD(0x0F,0x01,0x01);

  // move eyes
  eyesLeft(1000);
  eyesRight(1000);
  eyesMiddle(1000);
  eyesParanoid(500);

}

void loop() {

  eyesMiddle(300);

  myMovement=random(1, 5);

  if (myMovement==1) {
    Serial.println("Eyes left");
    eyesLeft(250);
  }
  else
  {
    Serial.println("Eyes right");
    eyesRight(250);
   }

   // Play audio
  myRandom=random(2,howManyAudios);
  Serial.println("Started audio");
  execute_CMD(0x0F,0x01,(byte)myRandom);
  delay(200);


  while (digitalRead(busyPin) == 0) {
    talkMovement();
    myRandom=random(150,300);
    delay(myRandom);
  }

 Serial.println("");
 Serial.println("Finished audio");


  myRandom=random(2000,5000);
  delay(myRandom);
}

void talkMovement()
{
  if (mouthOpen==0){
    myRandom=random(90,100);
    myservoMouth.write(myRandom);
    mouthOpen=1;
    }
  else
  {
    myRandom=random(100,107);
    myservoMouth.write(myRandom);
    mouthOpen=0;
    }
}

void eyesLeft(int howLong)
{
  myservoEyeLeft.write(10);
  myservoEyeRight.write(10);
  delay(howLong);
}

void eyesRight(int howLong)
{
  myservoEyeLeft.write(40);
  myservoEyeRight.write(40);
  delay(howLong);
}

void eyesMiddle(int howLong)
{
  myservoEyeLeft.write(20);
  myservoEyeRight.write(20);
  delay(howLong);
}

void eyesParanoid(int howLong)
{
  myservoEyeLeft.write(10);
  myservoEyeRight.write(10);
  delay(howLong);
  myservoEyeLeft.write(40);
  myservoEyeRight.write(40);
  delay(howLong);
  myservoEyeLeft.write(10);
  myservoEyeRight.write(10);
  delay(howLong);
  myservoEyeLeft.write(40);
  myservoEyeRight.write(40);
  delay(howLong);
  myservoEyeLeft.write(30);
  myservoEyeRight.write(30);

}

 void playFirst()
{
  execute_CMD(0x3F, 0, 0); // query device
  delay(500);
  setVolume(25);
  delay(500);

}

void pause()
{
  execute_CMD(0x0E,0,0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1);
  delay(500);
}

void playNext()
{
  execute_CMD(0x01,0,1);
  delay(500);
}

void playPrevious()
{
  execute_CMD(0x02,0,1);
  delay(500);
}

void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume);
  delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
      // Excecute the command and parameters
      {
      // Calculate the checksum (2 bytes)
      word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
      // Build the command line
      byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
      Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
      //Send the command line to the module
      for (byte k=0; k<10; k++)
      {
      mySerial.write( Command_line[k]);
      }

}
