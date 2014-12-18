
// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservoL;  // create servo object to control a servo 
Servo myservoR; //  // create servo object to control a servo 
 
int potpin = 0;  // analog pin used to connecti the potentiometer
int val;    // variable to read the value from the analog pin 

int[] sensors = { 3, 4, 5, 6, 7};
int[] sensorVals = {0, 0, 0, 0, 0};

 
void setup() 
{ 
  myservoL.attach(12);  // attaches the servo on pin 9 to the servo object 
  myservoR.attach(13);  // attaches the servo on pin 9 to the servo object 
  
  pinMode(pushButton, INPUT);
  
  for (int i=0; i<5; i++) {
    pinMode(sensors[i], INPUT);
  }
} 
 
void fw(int sp) {
  myservoL.write(90 + sp);                  // sets the servo position according to the scaled value 

  myservoR.write(90 - sp);                  // sets the servo position according to the scaled value   

} 
 
void left(int sp) {
  myservoL.write(90);                  // sets the servo position according to the scaled value 

  myservoR.write(90 - sp);                  // sets the servo position according to the scaled value   

} 
void right(int sp) {
  myservoL.write(90 + sp);                  // sets the servo position according to the scaled value 

  myservoR.write(90);                  // sets the servo position according to the scaled value   

} 
 
 
void loop() 
{ 
  
  for (int i=0; i<5; i++) {
    sensorVals[i] = digitalRead(sensors[i]);
  } 
  
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  
  if (sensorVals[1]==0) {
    left(10);
  } else fw(10);
                    // sets the servo position according to the scaled value   
  delay(15);                           // waits for the servo to get there 
} 
