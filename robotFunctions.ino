int servoPin;

void moveLeft()
{
  changeMoveState(MOV_LEFT);
  motorForward(MOTOR_RIGHT,  moveSpeed);
  motorReverse(MOTOR_LEFT, moveSpeed);
}

void moveRight()
{
  changeMoveState(MOV_RIGHT);
  motorReverse(MOTOR_RIGHT,  moveSpeed);
  motorForward(MOTOR_LEFT, moveSpeed);
}

void moveForward()
{
  changeMoveState(MOV_FORWARD);
  motorForward(MOTOR_LEFT,  moveSpeed);
  motorForward(MOTOR_RIGHT, moveSpeed);
}

void moveBackward()
{
  changeMoveState(MOV_BACK);
  motorReverse(MOTOR_LEFT, moveSpeed);
  motorReverse(MOTOR_RIGHT, moveSpeed);  
}

void moveStop()
{
  changeMoveState(MOV_STOP);
  motorStop(MOTOR_LEFT);
  motorStop(MOTOR_RIGHT);
}

void moveSetSpeed(int speed)
{
  motorSetSpeed(MOTOR_LEFT, speed) ;
  motorSetSpeed(MOTOR_RIGHT, speed) ;
  moveSpeed = speed; // save the value
}

void changeMoveState(int newState)
{
  if(newState != moveState)
  {
    Serial.print("Changing move state from "); Serial.print( states[moveState]);
    Serial.print(" to "); Serial.println(states[newState]);
    moveState = newState;
  } 
}


void softServoAttach(int pin)
{
   servoPin = pin;
   pinMode(pin, OUTPUT); 
}

// writes given angle to servo for given delay in milliseconds
void softServoWrite(int angle, long servoDelay)
{
  int pulsewidth = map(angle, 0, 180, 544, 2400); // width in microseconds
  do {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servoPin, LOW);
    delay(20); // wait for 20 milliseconds
    servoDelay -= 20; 
  } while(servoDelay >=0);
}

int pingGetDistance(int pingPin)
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, cm, inches;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH, 20000); // if a pulse does not arrive in 20 ms then the ping sensor is not connected
  if(duration >=20000) {
    duration = 10000;
    //Serial.println("Time Out");
  } else {
    //Serial.println("We got it");
  }
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  //Serial.print(cm);
  //Serial.println(" cm");
  inches = (cm * 10) / 25;
  //Serial.print(inches);
  //Serial.println(" inches");
  return inches; // convert cm to inches
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}


// function to indicate numbers by flashing the built-in LED
void blinkNumber( byte number) {
   pinMode(LED_PIN, OUTPUT); // enable the LED pin for output
   while(number--) {
     digitalWrite(LED_PIN, HIGH); delay(100);
     digitalWrite(LED_PIN, LOW);  delay(400);
   }
}
