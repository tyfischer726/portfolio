#include <Servo.h>;
//Front Motors
int rightFrontBackward = 2, rightFrontForward = 3; 
int leftFrontBackward = 4, leftFrontForward = 5;
//Rear Motors
int leftBackBackward = 8, leftBackForward = 9;
int rightBackBackward = 10, rightBackForward = 11;
//Object Clearing Arm (OCA)
Servo OCA;
int OCAPin = 7;
float GetObjectHeight();
float objectHeight;
//Tilt Sensor
int tiltPin = A0;
float slopeSensed;
float GetSlope();
int SIZE=0;
int* groundSlopes = new int[SIZE];
//Distance Sensor
int triggerPin = 12, echoPin = 13;
float time, distanceSensed, objectDistance = 100;
float GetDistance();
//Survey Area
int GetLength();
int areaLength;
int GetWidth();
int areaWidth;
int surveyLengths;
//Bot dimensions
float botWidth = .2032; 						//m
float botHeight = .127; 						//m
float wheelCircumference = 2*3.14159*0.04445; 	//m
float botSpeed = 64.51543/60;					//meters per second
//Bot & object location
float distanceTraveled = 0;						//m
int lengthsCompleted = 0;
int GetObjectXLocation();
int objectXLocation;
int GetObjectYLocation();
int objectYLocation;
int objectLocation[2]={};
void SendLocation();
//Time step for data gathering (every second)
long int t1;
long int t2;
float stepDistance;
int counter = 0;
int step = 0;

///////////////////////////////////////////////////////

void setup()
{
  //Motors
  pinMode(leftFrontForward, OUTPUT);
  pinMode(leftFrontBackward, OUTPUT);
  pinMode(rightFrontForward, OUTPUT);
  pinMode(rightFrontBackward, OUTPUT);
  pinMode(leftBackForward, OUTPUT);
  pinMode(leftBackBackward, OUTPUT);
  pinMode(rightBackForward, OUTPUT);
  pinMode(rightBackBackward, OUTPUT);
  
  //OCA Servo
  pinMode(OCAPin, OUTPUT);
  OCA.attach(OCAPin);
  OCA.write(0);
  
  //Tilt Sensor
  pinMode(tiltPin, INPUT);
  
  //Distance Sensor
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
  
  //Get survey area dimensions (from Central Hub)
  areaLength = GetLength();
  areaWidth = GetWidth();
  surveyLengths = areaWidth/(botWidth);
  Serial.print(surveyLengths);
  Serial.println(" lengths to complete.");
  SIZE = areaLength/botSpeed;				//Size of groundSlopes array.
  											//Adds a new data point every second.
}

///////////////////////////////////////////////////////
void loop()
{
  if (lengthsCompleted < surveyLengths){
    if (distanceTraveled == 0){
      Serial.println();
      Serial.print("Starting length ");
      Serial.println(lengthsCompleted + 1);
      step = 0;
    }
    if (objectDistance > 17){
      while (distanceTraveled < areaLength && objectDistance > 17){
        t1 = millis();
        GoForward();
        delay(1000);
        t2 = millis();
        stepDistance = ((t2-t1)/1000)*botSpeed;
        distanceTraveled += stepDistance;
        groundSlopes[step] = GetSlope();
        step += 1;
        objectDistance = GetDistance();
      }
      Stop();
      delay(500);
      if (distanceTraveled >= areaLength){
        lengthsCompleted += 1;
        Serial.print(lengthsCompleted);
        Serial.println(" lengths completed.");
        Serial.print("Slope data: ");
        for (int i=0; i < SIZE; i++){
          Serial.print(groundSlopes[i]);
          Serial.print(", ");
        }
        Serial.println();
        UTurn();
        distanceTraveled = 0;
        delay(2000);
      }
    }
    else if (counter == 0){
      counter += 1;
      objectHeight = GetObjectHeight();
      if (objectHeight <= botHeight)
      	ClearObject();
      objectDistance = GetDistance();
      if (objectDistance > 17){
        Serial.println("Continuing length.");
        counter = 0;
      }
    }
    else {
      SendLocation();
      GoAround();
      counter = 0;
      objectDistance = GetDistance();
      if (objectDistance > 17)
        Serial.println("Continuing length.");
    }
  }
  else{
    Serial.println();
    Serial.println("Surveying done.");
    Serial.print(lengthsCompleted);
    Serial.println(" lengths completed.");
    while (0==0){
    }
  }
}

///////////////////////////////////////////////////////

//Functions
void GoForward(){
  digitalWrite(leftFrontForward, HIGH);
  digitalWrite(leftFrontBackward, LOW);
  digitalWrite(leftBackForward, HIGH);
  digitalWrite(leftBackBackward, LOW);
  
  digitalWrite(rightFrontForward, HIGH);
  digitalWrite(rightFrontBackward, LOW);
  digitalWrite(rightBackForward, HIGH);
  digitalWrite(rightBackBackward, LOW);
}

void Stop(){
  digitalWrite(leftFrontForward, LOW);
  digitalWrite(leftFrontBackward, LOW);
  digitalWrite(leftBackForward, LOW);
  digitalWrite(leftBackBackward, LOW);
  
  digitalWrite(rightFrontForward, LOW);
  digitalWrite(rightFrontBackward, LOW);
  digitalWrite(rightBackForward, LOW);
  digitalWrite(rightBackBackward, LOW);
}

void TurnLeft(){
  digitalWrite(leftFrontForward, LOW);
  digitalWrite(leftFrontBackward, HIGH);
  digitalWrite(leftBackForward, LOW);
  digitalWrite(leftBackBackward, HIGH);
  
  digitalWrite(rightFrontForward, HIGH);
  digitalWrite(rightFrontBackward, LOW);
  digitalWrite(rightBackForward, HIGH);
  digitalWrite(rightBackBackward, LOW);
}

void TurnRight(){
  digitalWrite(leftFrontForward, HIGH);
  digitalWrite(leftFrontBackward, LOW);
  digitalWrite(leftBackForward, HIGH);
  digitalWrite(leftBackBackward, LOW);
  
  digitalWrite(rightFrontForward, LOW);
  digitalWrite(rightFrontBackward, HIGH);
  digitalWrite(rightBackForward, LOW);
  digitalWrite(rightBackBackward, HIGH);
}

void ClearObject(){
  Serial.println("Clearing object.");
  for (int i=0; i<3; i++){
    OCA.write(90);
  	delay(1500);
  	OCA.write(0);
  	delay(1500);
  }
}

void UTurn(){
  Serial.println("Turning around.");
  TurnRight();
  delay(1000);
  GoForward();
  delay(1000);
  TurnRight();
  delay(1000);
  Stop();
  delay(500);
  counter = 0;
}

void GoAround(){
  Serial.println("Going around object.");
  TurnRight();
  delay(500);
  GoForward();
  delay(500);
  TurnLeft();
  delay(500);
  GoForward();
  delay(500);
  TurnLeft();
  delay(500);
  GoForward();
  delay(500);
  TurnRight();
  delay(500);
  Stop();
  delay(1000);
}

float GetSlope(){
  slopeSensed = analogRead(tiltPin);
  	
  return slopeSensed;
}

float GetDistance(){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  time = pulseIn(echoPin, HIGH)/2000000.0;
  distanceSensed = time*34300.0;
  
  return distanceSensed;
}

int GetLength(){
  Serial.print("Enter the length of the area in meters: ");
  
  while (Serial.available() == 0){
  }
  
  areaLength = Serial.parseInt();
  Serial.println(areaLength);
  return areaLength;
}

int GetWidth(){
  Serial.print("Enter the width of the area in meters: ");
  
  while (Serial.available() == 0){
  }
  
  areaWidth = Serial.parseInt();
  Serial.println(areaWidth);
  return areaWidth;
}

float GetObjectHeight(){
  Serial.println("Object encountered. Getting drone data...");
  while (Serial.available() == 0){
  }
  
  objectHeight = Serial.parseFloat();
  Serial.println(objectHeight);
  return objectHeight;
}

int GetObjectXLocation(){
  objectXLocation = (lengthsCompleted+1)*botWidth;
  
  return objectXLocation;
}

int GetObjectYLocation(){
  objectYLocation = distanceTraveled;
    
  return objectYLocation;
}

void SendLocation(){
  Serial.println("Sending object location to swarm...");
  objectLocation[0] = GetObjectXLocation();
  objectLocation[1] = GetObjectYLocation();
  
  Serial.print("Object at: (");
  Serial.print(objectLocation[0]);
  Serial.print(", ");
  Serial.print(objectLocation[1]);
  Serial.println(")");
}
