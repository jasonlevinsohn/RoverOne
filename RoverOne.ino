// Site to create a Class for the utility functions
// http://n0m1.com/2012/05/17/writing-your-own-arduino-libraries/


#include <AFMotor.h>  // adafruit motor shield library (modified my mm)
#include "RobotMotor.h"    // 2wd or 4wd motor library
#include <RobotUtils.h> //J-Lev's Robot Functions

#include "robotDefines.h"  // global defines

// Pin 
const int PING_PIN = 10;
const int LED_PIN = 13;
const int SERVO_PIN = 9;

//Motor Defines
int moveSpeed = 0;
int moveState = MOV_STOP;
int distance;
boolean doOnce = true;
boolean moving = true;
boolean thresholdReached = false;
boolean movingBackward = false;

//Checking direction vars
int leftCheckDistance;
int rightCheckDistance;


//Servo Defines
const int servoAngle[] = {165,  5,   80};

void setup() {
  
  //Activate Serial Port
  Serial.begin(9600);  
  
  //Opening Ports required for Arduino
  blinkNumber(8);      
  
  //Initiate Servo
  softServoAttach(SERVO_PIN);  
  Serial.println("Initiating Rotating Servo...");
  
  // Initiate Motors
  motorBegin(MOTOR_LEFT); 
  Serial.println("Initiating Left Motors..."); 
  motorBegin(MOTOR_RIGHT); 
  Serial.println("Initiating Right Motors..."); 
  moveStop();
  
  // Set Initial States
  moveSetSpeed(MIN_SPEED);
  //moveSetSpeed(MIN_SPEED + 10);
  softServoWrite(90, 1000);  //Centers the servo
  Serial.println("Initiation Complete.");
  
  
  
  
}

void loop() {
  
  //TODO: Implement a run once algorithm so do...while(false); maybe?
  
  //Serial.println("Running Servo Test Program...");
  Serial.println("Running Ping Test Program...");
  delay(4000);
  
  
 
  if(doOnce) {
     Serial.println("Looking Left...");
  softServoWrite(servoAngle[0], 700);
  delay(1000);
  
  Serial.println("Looking Right...");
  softServoWrite(servoAngle[1], 700);
  delay(1000);
  
  Serial.println("Looking Straight...");
  softServoWrite(servoAngle[2], 700);

  Serial.println("We are done here");
  delay(2000);  
  
  doOnce = false;
  
  }
  
  
 
  // Here we have to stop the motors to get a distance
  // otherwise the ping will not be received.
  if(moving) {
    moveStop();
  }
  //distance = pingGetDistance(PING_PIN);
  distance = checkCumDistance();
  
  if(moving) {
   moveForward(); 
  }
  
  
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("Starting Main Test");
  delay(2000);
  
  
  if (distance < 24 || movingBackward == true) {
    Serial.println("We are stopping");
    Serial.println(distance);
    Serial.println(movingBackward);
    moveStop(); 
    movingBackward = false;
    moving = false;  
    thresholdReached = true;
    
    // Let's check to see if we can move left.
    leftCheckDistance = checkDistanceLeft(true);
    
    if(leftCheckDistance > 24) {
      // Turn left and move forward
      turn45DegLeft();
      moving = true;
      
    } else {
      rightCheckDistance = checkDistanceLeft(false);
      if(rightCheckDistance > 24) {
       // Turn right and move forward
       turn45DegRight();
       moving = true;
      } else {
       // Go back 10 feet and try again
       Serial.println("We should go back. There is no were to go.");
       softServoWrite(servoAngle[2], 700);
       movingBackward = true;
       moveBackward();
       delay(5000);
       moveStop();
       moving = false;
      }
    }
    
    
  } else {
    moveForward(); 
    moving = true;
    
  }
  
}

int checkDistanceLeft(boolean dir) {
  int cumDistance;
  
  if(dir) {
    Serial.println("Checking Distance Left");
    delay(2000);
  } else {
    Serial.println("Checking Distance Right");
    delay(2000);
  }
  
  delay(1000);
  if(dir) {
    // Look Left
    softServoWrite(servoAngle[0], 1000);
  } else {
    // Look Right
    softServoWrite(servoAngle[1], 1000);  
  }
  
  cumDistance = checkCumDistance();
  return cumDistance;
    

}

int checkCumDistance() {
    int sumDistance = 0;
    for(int i = 0; i < 3; i++) {
     //We are testing here for error with motors and pings
     delay(100);
     distance = pingGetDistance(PING_PIN); 
     sumDistance = sumDistance + distance;
     delay(500);
     Serial.print("Sum Distance is: ");
     Serial.println(sumDistance);
    }
    int cum = sumDistance / 3;
    
    Serial.print("Cum distance is ");
    Serial.print(cum);
    Serial.println(" cm");
    return cum;
}

void turn45DegLeft() {
      Serial.println("turning left now");
      softServoWrite(servoAngle[2], 700);
      delay(1000);
      moveLeft();
      delay(2000);
      moveStop();
      Serial.println("LEFT turn complete");
      delay(7000);
}

void turn45DegRight() {
  Serial.println("turning right now");
  softServoWrite(servoAngle[2], 700);
  delay(1000);
  moveRight();
  delay(2000);
  moveStop();
  Serial.println("RIGHT turn complete");
  delay(7000);
}
