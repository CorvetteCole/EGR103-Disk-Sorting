

// include the Servo library
#include <Servo.h>
#include <Stepper.h>

int stepsPerRevolution = 4096; // 28BYJ-48 output shaft steps per rev
int turnTableGearRatio = 4.1; // not exact, need to check on this

int classifiedDisk;
double roughnessSensorBackground;

int sorterSteps;

Servo diskViewerServo; 
Servo sorterServo; 
Stepper diskViewerStepper(stepsPerRevolution, 6, 7, 8, 9);
Stepper sorterStepper(stepsPerRevolution * turnTableGearRatio, 10, 11, 12, 13);

// number of steps per location (multiplied by turnTableGearRatio):
// 0 - 410 
// 1 - 1229
// 2 - 2048
// 3 - 2867
// 4 - 3686
int materialSteps[5] = {410, 1229, 2048, 2867, 3686};

void setup() {
  diskViewerServo.attach(2); 
  sorterServo.attach(3);
  diskViewerStepper.setSpeed(700);
  sorterStepper.setSpeed(700);
  Serial.begin(9600); 
  
  // average background roughness sensor values to calibrate against ambient. 
  // maybe switch to mode later
  double roughnessTotal = 0;
  for (int i = 0; i < 10; i++){
	  roughnessTotal += analogRead(A0); // read roughness sensor and add to roughnessTotal
	  delay(50); // delay time to let it read properly
  }
  roughnessSensorBackground = roughnessTotal / 10.0;
}

void loop() {
	// read roughness sensor to find value that is not background noise (read for at least like 10 times and find mode)
	// if roughness sensor detects the transparent side of disk, set the below to true
	bool roughnessSensorTriggered = false;
    bool shouldRotateDisk = false;
	while (!roughnessSensorTriggered){
		// if voltage difference is half a volt different than background, do stuff
		if (abs(analogRead(A0) - roughnessSensorBackground > .5)){
			roughnessSensorTriggered = true;
		// #TODO better detection of currently viewed side of disk
			if (analogRead(A0) > 1.5) {
				shouldRotateDisk = true;
			}
		}
	}
	roughnessSensorTriggered = false;
	
	
	diskViewerServo.write(170); // open feeder servo
	delay(200);                 // delay for disk to fall in
	diskViewerServo.write(0);   // close feeder servo
	
	delay(100);     // delay for disk to settle (may not be needed)
	if (shouldRotateDisk){
		diskViewerStepper.step(stepsPerRevolution / 2);
	}
	Serial.write(1); // tell Raspberry Pi to start looking
	while (!Serial.available()){
		// delay for amount of time, then Arduino will check if serial is available after
		delay(50);
	}
	// get classification
	// cloth - 0
    // metal - 1
    // sandpaper - 2
    // wood - 3
	int classifiedMaterial = Serial.parseInt();
	int confidence = Serial.parseInt();
	Serial.flush();
	
	if (confidence < .95){
		diskViewerStepper.step(-stepsPerRevolution / 2);
		shouldRotateDisk = true;
		Serial.write(1);
		while (!Serial.available()){
		// delay for amount of time, then Arduino will check if serial is available after
		delay(50);
		}
		int newClassifiedMaterial = Serial.parseInt();
		int newConfidence = Serial.parseInt();
		Serial.flush();
		if (newConfidence > confidence){
			classifiedMaterial = newClassifiedMaterial;
		}
	}
	
	if (shouldRotateDisk){

		diskViewerStepper.step(-stepsPerRevolution / 2);
		shouldRotateDisk = false;
	}
	
	// calculate and move stepper for sorter according to classifiedDisk result

	// really inefficient sorter move code, #TODO optimize this
	int moveSteps = stepsPerRevolution * turnTableGearRatio - sorterSteps + materialSteps[classifiedMaterial] * turnTableGearRatio;
	sorterStepper.step(moveSteps);
	sorterSteps += moveSteps;
	if (sorterSteps > stepsPerRevolution * turnTableGearRatio){
		sorterSteps -= stepsPerRevolution * turnTableGearRatio;
	}
	
	sorterServo.write(170); 	// open servo on disk holder
	delay(200);					// delay for disk to come out
	sorterServo.write(0);		// close servo on disk holder
}
