// Creepy Animatronic Head with spare parts
// Useless machines talk, Feria del libro, May 2025
// Roni Bandini, @ronibandini
// MIT License


#include <Servo.h>
#include <DFRobot_GestureFaceDetection.h>
#define DEVICE_ID  0x72 

DFRobot_GestureFaceDetection_I2C gfd(DEVICE_ID);
char str[100];

Servo myservo; 
Servo servoMouth; 

# Settings
int relayPin=8;
int servoPin=9;
int servoMouthPin=10;
int loopDelay=5000;
int headCenter=120;
int headPosition=1;

int pos = 0;
long randNumber;
int headMovDelay;


void cabeza(){    
  headMovDelay = random(50, 200);

  if (headPosition==1){
    for (pos = headCenter; pos >= 50; pos -= 1) {     
      myservo.write(pos);              
       int gestureType = gfd.getGestureType();               
        if (gestureType==4){
          eyesOn();
        }
      delay(headMovDelay);                            
    }  
  }

  if (headPosition==2){
    for (pos = 50; pos <=headCenter; pos += 1) {     
      myservo.write(pos);   
      int gestureType = gfd.getGestureType();               
        if (gestureType==4){
          eyesOn();
        }         
      delay(headMovDelay);                            
    }  
  }

  if (headPosition==3){

  for (pos = headCenter; pos <165; pos += 1) {  
    myservo.write(pos);  
    int gestureType = gfd.getGestureType();               
        if (gestureType==4){
          eyesOn();
        }           
    delay(headMovDelay);                            
    }  

  }

   if (headPosition==3){

  for (pos = 165; pos <headCenter; pos -= 1) {  
    myservo.write(pos);  
    int gestureType = gfd.getGestureType();               
        if (gestureType==4){
          eyesOn();
        }           
    delay(headMovDelay);                            
    }  

  }

  headPosition=headPosition+1;
  if (headPosition==5) {headPosition=1;}

  eyesOff();
}

void boca(){

      Serial.println("Mouth...");
      int howMany = random(3, 8);
      int openTo=0;

      for (pos = 1; pos <=howMany; pos += 1) { 
            openTo = random(105, 125);
            servoMouth.write(openTo);   
            randNumber = random(90, 250);
            delay(randNumber);
            openTo = random(105, 125);
            servoMouth.write(openTo);  
            randNumber = random(80, 250);
            delay(randNumber);
            servoMouth.write(115);                     
       } 

     
       
      
}

void eyesOn(){
 digitalWrite(relayPin,1);
}
void eyesOff(){
 digitalWrite(relayPin,0);
}

//////////////////////////////////////////////////////////////// SETUP
void setup() {

  randomSeed(analogRead(0));

  myservo.attach(servoPin);
  servoMouth.attach(servoMouthPin);

  pinMode(relayPin, OUTPUT);

  gfd.begin(&Wire);
  Serial.begin(115200);
  gfd.setFaceDetectThres(70);
  gfd.setGestureDetectThres(60);    
  gfd.setDetectThres(100);

  myservo.write(headCenter);
  servoMouth.write(115);

  Serial.begin(115200);
  Serial.println("");
  Serial.println("Creepy Animatronic Head Started...");


}


void loop() {

  if(gfd.getFaceNumber() > 0){

        Serial.println("Face detected");

        int gestureType = gfd.getGestureType();
        int gestureScore = gfd.getGestureScore();
        
        if (gestureType==4){
          Serial.println("Peronist fingers");
          Serial.println("Eyes...");
          eyesOn();
          randNumber = random(200, 400);
          delay(randNumber);
          eyesOff();          
          }
          else{
               Serial.println("Standard routine");
               randNumber = random(1, 4);

                if (randNumber==1 or randNumber==2){
                    cabeza();
                }
                
                  if (randNumber==3){                    
                    boca();
                }

                delay(loopDelay); 

          }        


  } // there is a face
  
 delay(50);
  

}
