// Site to create a Class for the utility functions
// http://n0m1.com/2012/05/17/writing-your-own-arduino-libraries/


//TODO:
//Raise distance when checking left or right
//Increase speed when turning

#include <AFMotor.h>  // adafruit motor shield library (modified my mm)
#include "RobotMotor.h"    // 2wd or 4wd motor library
#include <RobotUtils.h> //J-Lev's Robot Functions

#include "robotDefines.h"  // global defines

// Pin 
const int PING_PIN = 10;
const int LED_PIN = 13;
const int SERVO_PIN = 9;

//Motor Defines
int moveSpeed = 40;
int turnSpeed = 60;
const int stopDistance = 10;
const int dontTurn = 13;
const int turnTime = 1300;


int moveState = MOV_STOP;
int distance = 0;

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
  moveSetSpeed(moveSpeed);
  //moveSetSpeed(MIN_SPEED + 10);
  softServoWrite(90, 1000);  //Centers the servo
  Serial.print("Initiation Complete. Min Speed is: ");
  Serial.println(moveSpeed);
  
  
  
  
}

void loop() {
  
  //TODO: Implement a run once algorithm so do...while(false); maybe?
  
  //Serial.println("Running Servo Test Program...");
  //Serial.println("Running Ping Test Program...");
  //delay(3000);
  
  
 
  if(doOnce) {
     Serial.println("Looking Left...");
  softServoWrite(servoAngle[0], 700);
  delay(700);
  
  Serial.println("Looking Right...");
  softServoWrite(servoAngle[1], 700);
  delay(700);
  
  Serial.println("Looking Straight...");
  softServoWrite(servoAngle[2], 700);

  Serial.println("We are done here");
  delay(700);  
  
  doOnce = false;
  
  }
  
  
 
  // Here we have to stop the motors to get a distance
  // otherwise the ping will not be received.
  //if(moving) {
   // moveStop();
  //}
  //distance = pingGetDistance(PING_PIN);
  distance = checkCumDistance();
  
  //if(moving) {
   //moveForward(); 
  //}
  
  
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" Speed: ");
  Serial.println(MIN_SPEED);
  if (((distance < stopDistance) && (distance > 2)) || movingBackward == true) {
    Serial.print("We are stopping");
    Serial.print(distance);
    Serial.println(" cm");
    moveStop(); 
    movingBackward = false;
    moving = false;  
    thresholdReached = true;
    
    // Let's check to see if we can move left.
    leftCheckDistance = checkDistanceLeft(true);
    
    if(leftCheckDistance > dontTurn) {
      // Turn left and move forward
      moveSetSpeed(turnSpeed);
      turn45DegLeft();
      moveSetSpeed(moveSpeed);
      moving = true;
      
    } else {
      rightCheckDistance = checkDistanceLeft(false);
      if(rightCheckDistance > dontTurn) {
       // Change speed for turning. 
       // Turn right and move forward. 
       moveSetSpeed(turnSpeed);
       turn45DegRight();
       moveSetSpeed(moveSpeed);
       moving = true;
      } else {
       // Go back 10 feet and try again
       Serial.println("We should go back. There is no were to go.");
       softServoWrite(servoAngle[2], 700);
       movingBackward = true;
       moveBackward();
       delay(3000);
       moveStop();
       moving = false;
      }
    }
    
    
  } else {
    Serial.println("Distance Checked: MOVING FORWARD");
    moveForward(); 
    moving = true;
    
  }
  
}

int checkDistanceLeft(boolean dir) {
  int cumDistance;
  
  if(dir) {
    Serial.println("Checking Distance Left");
    delay(1000);
  } else {
    Serial.println("Checking Distance Right");
    delay(1000);
  }
  
  delay(1000);
  if(dir) {
    // Look Left
    softServoWrite(servoAngle[0], 1000);
  } else {
    // Look Right
    softServoWrite(servoAngle[1], 1000);  
  }
  
  delay(500);
  cumDistance = checkCumDistance();
  
  Serial.print("Distance is: ");
  Serial.print(cumDistance);
  Serial.println(" inches");
  delay(1000);
  return cumDistance;
    

}

int checkCumDistance() {
    int sumDistance = 0;
    int cum = 0;
    for(int i = 0; i < 3; i++) {
     //We are testing here for error with motors and pings
     delay(10);
     distance = pingGetDistance(PING_PIN); 
     sumDistance = sumDistance + distance;
     delay(10);
    
    }
    cum = sumDistance / 3;
    
        return cum;
}

void turn45DegLeft() {
      Serial.println("turning left now");
      softServoWrite(servoAngle[2], 700);
      delay(1000);
      moveLeft();
      delay(turnTime);
      moveStop();
      Serial.println("LEFT turn complete");
      delay(3000);
}

void turn45DegRight() {
  Serial.println("turning right now");
  softServoWrite(servoAngle[2], 700);
  delay(1000);
  moveRight();
  delay(turnTime);
  moveStop();
  Serial.println("RIGHT turn complete");
  delay(3000);
}
