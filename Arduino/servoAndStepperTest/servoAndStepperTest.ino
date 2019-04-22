/*
  Arduino Starter Kit example
  Project 5 - Servo Mood Indicator

  This sketch is written to accompany Project 5 in the Arduino Starter Kit

  Parts required:
  - servo motor
  - 10 kilohm potentiometer
  - two 100 uF electrolytic capacitors

  created 13 Sep 2012
  by Scott Fitzgerald

  http://www.arduino.cc/starterKit

  This example code is part of the public domain.
*/

// include the Servo library
#include <Servo.h>
#include <Stepper.h>

Servo feederServo;  // create a servo object
Servo sorterServo;

int angle;   // variable to hold the angle for the servo motor

// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 32; 
 
//  Amount of Gear Reduction
const float GEAR_REDUCTION = 16.032;

// Gearing of the sorter
const float GEAR_SORTER_RATIO = 4.36;
 
// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_REDUCTION;
 
// Define Variables
 
// Number of Steps Required
int stepsRequired;
 
// Create Instance of Stepper Class
// Specify Pins used for motor coils
// The pins used are 8,9,10,11 
// Connected to ULN2003 Motor Driver In1, In2, In3, In4 
// Pins entered in sequence 1-3-2-4 for proper step sequencing
Stepper stepperMotor1(STEPS_PER_REV, 6, 7, 8, 9);
Stepper stepperMotor2(STEPS_PER_REV, 10, 11, 12, 13);

void setup() {
  feederServo.attach(5); 
  feederServo.write(180);
  sorterServo.attach(3);
  sorterServo.write(180);

  Serial.begin(9600); // open a serial connection to your computer
}

void loop() {

  
  // print out the angle for the servo motor
  //Serial.print("moving to 180 degrees...");

   // Rotate CW 1/2 turn slowly
  //StepsRequired  =  STEPS_PER_OUT_REV / 2; 
  //stepperMotor1.setSpeed(100);   
  //stepperMotor1.step(StepsRequired);

  //stepperMotor2.setSpeed(100);   
  //stepperMotor2.step(StepsRequired);
  
  // set the servo position
  //servo1.write(180);

  // wait for the servo to get there
  //delay(1000);

    // print out the angle for the servo motor
  //Serial.print("moving to 0 degrees...");

    // Rotate CCW 1/2 turn quickly
  while (!Serial.available()){
    delay(50);
  }
  int partCode = Serial.parseInt();
  
  
  if (partCode == 0){  
    Serial.print("f or r (forward/reverse):");
    while (!Serial.available()){
    delay(50);
  }
  char dir = Serial.read();
  int motorDirection;
  if (dir == 'f'){
    motorDirection = 1;
  } else {
    motorDirection = -1;
  }
    Serial.print("\n moving sorter stepper 180 degrees \n");
    stepsRequired  =  motorDirection * STEPS_PER_OUT_REV  * GEAR_SORTER_RATIO; 
    stepperMotor1.setSpeed(700);  
    stepperMotor1.step(stepsRequired);
  } else if (partCode == 1){
    Serial.print("f or r (forward/reverse):");
    while (!Serial.available()){
    delay(50);
  }
  char dir = Serial.read();
  int motorDirection;
  if (dir == 'f'){
    motorDirection = 1;
  } else {
    motorDirection = -1;
  }
    Serial.print("\n moving disk holder stepper 360 degrees \n");
    stepsRequired  =  (motorDirection * STEPS_PER_OUT_REV) / 2; 
    stepperMotor2.setSpeed(700);  
    stepperMotor2.step(stepsRequired);
  } else if (partCode == 2 || partCode == 3){
    Serial.print("angle for servo (150 is open, 180 is closed) : ");
    while (!Serial.available()){
    delay(50);
    }
  }
  if (partCode == 2){
  int angle = Serial.parseInt();
    Serial.print("\n moving feeder servo ");
    Serial.print(angle);
    Serial.print(" degrees \n");
    feederServo.write(angle);
  } else if (partCode == 3){
      int angle = Serial.parseInt();
    Serial.print("\n moving sorter servo ");
    Serial.print(angle);
    Serial.print(" degrees \n");
    sorterServo.write(angle);
  }

  //stepperMotor2.setSpeed(700);  
  //stepperMotor2.step(stepsRequired);
  
  // set the servo position
  //servo1.write(0);

  // wait for the servo to get there
  //delay(1000);
}
