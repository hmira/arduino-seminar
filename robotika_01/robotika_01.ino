
// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservoL;  // create servo object to control a servo 
Servo myservoR; //  // create servo object to control a servo 
 
int potpin = 0;  // analog pin used to connecti the potentiometer
int val;    // variable to read the value from the analog pin 

//int sensors[] = {11, 9, 10};
//int sensorVals[] = {0, 0, 0};

//int sensors[] = {7, 6, 5, 4, 3};
int sensors[] = {3, 4, 5, 6, 7};
int sensorVals[] = {0, 0, 0, 0, 0};
const int BUTTON_SENSOR = 8;

//int sensorCount = 3;
int sensorCount = 5;

//int LEFT_SENSOR = sensors[0];
//int CENTER_SENSOR = sensors[1];
//int RIGHT_SENSOR = sensors[2];

int TURN_LEFT_SENSOR = sensors[4];
int TURN_RIGHT_SENSOR = sensors[0];

int LEFT_SENSOR = sensors[3];
int CENTER_SENSOR = sensors[2];
int RIGHT_SENSOR = sensors[1];

//int LEFT_MARK = sensors[0];
//int RIGHT_MARK = sensors[4];

int INERTIA = 900;
int LAST_SENSOR = CENTER_SENSOR;

int inertia = INERTIA;
int BLACK_FLAG = 0;
 
void setup() 
{ 
  
  Serial.begin(9600);
  
  

  //myservoL.attach(12);  // attaches the servo on pin 9 to the servo object 
  //myservoR.attach(13);  // attaches the servo on pin 9 to the servo object 
  
  myservoL.attach(13);  // attaches the servo on pin 9 to the servo object 
  myservoR.attach(12);  // attaches the servo on pin 9 to the servo object 

  //myservoL.attach(11);  // attaches the servo on pin 9 to the servo object 
  //myservoR.attach(10);  // attaches the servo on pin 9 to the servo object 
  
  //pinMode(pushButton, INPUT);
  
  for (int i=0; i<sensorCount; i++) {
    pinMode(sensors[i], INPUT);
  }
  
  
  pinMode(BUTTON_SENSOR, OUTPUT);
  digitalWrite(BUTTON_SENSOR, HIGH);
}

boolean buttonPressed() {
  return digitalRead(BUTTON_SENSOR) == 0;
}
 
void fw(int sp) {
  myservoL.write(90 - sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 + sp);                  // sets the servo position according to the scaled value   

} 

void fw_2(int sp) {
  myservoL.write(90 + sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 - sp);                  // sets the servo position according to the scaled value   

} 
 
void left(int sp) {
  myservoL.write(90 - sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 - (sp/2));                  // sets the servo position according to the scaled value   

} 

void left(int sp, int sec_sp) {
  myservoL.write(90 - sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 - sec_sp);                  // sets the servo position according to the scaled value   

}

void left_2(int sp, int sec_sp) {
  myservoL.write(90 + sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 + sec_sp);                  // sets the servo position according to the scaled value   

}

void left(int sp, int sec_sp, int inertia) {
  myservoL.write(90 - sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 - sec_sp + inertia / 10);                  // sets the servo position according to the scaled value   

}

void right(int sp) {
  myservoL.write(90 + (sp/2));                  // sets the servo position according to the scaled value 
  myservoR.write(90 + sp);                  // sets the servo position according to the scaled value   

} 

void right(int sp, int sec_sp) {
  myservoL.write(90 + sec_sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 + sp);                  // sets the servo position according to the scaled value   

} 

void right_2(int sp, int sec_sp) {
  myservoL.write(90 - sec_sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 - sp);                  // sets the servo position according to the scaled value   

} 

void right(int sp, int sec_sp, int inertia) {
  myservoL.write(90 + sec_sp - inertia / 10);                  // sets the servo position according to the scaled value 
  myservoR.write(90 + sp);                  // sets the servo position according to the scaled value   

} 

int _FALSE = 0;
int _TRUE = 1;

int PRIORITY_LEFT = 0xFEEDDEEF;
int PRIORITY_RIGHT = 0xFEEDBEEF;

int AFTER_CROSSING = _FALSE;

int priority = PRIORITY_RIGHT;

int left_block = 0;
int right_block = 0;

int FREE = 0;
int EXPECT_CROSSING_1 = 1;
int CROSSING_1 = 2;
int EXPECT_CROSSING_2 = 3;
int CROSSING_2 = 4;

int ON_START = 1;
int OFF_START = 0;
int pos = ON_START;

int ON_SPLIT = 1;
int OFF_SPLIT = 0;

int split_flag = OFF_SPLIT;

int WAITING_ON_PUSH = 255;
int SEEKING_START = 0;
int WAITING_ON_START = 1;
int READY_TO_START = 2;
int RUNNING = 3;
int FINISHED = 4;
int button_state = WAITING_ON_PUSH;

void loop() 
{
  if (button_state == WAITING_ON_PUSH) {
    if (buttonPressed()) {
      button_state = SEEKING_START;
    }
    else {
      fw_2(0);
      return;
    }
  }
  
  if (button_state == READY_TO_START) {
    if (buttonPressed()) {
      button_state = RUNNING;
    }
    else {
      fw_2(0);
      return;
    }
  }
  
  /*
  for (int i=0; i<sensorCount; i++) {
    sensorVals[i] = digitalRead(sensors[i]);
    Serial.print(sensorVals[i]);
  } 
  Serial.print("\n");
  */
  
  //Serial.print("\n");
  
  //val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  //val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  
  //delay(100);
  //Serial.print(inertia);
  //Serial.print(LAST_SENSOR);
  //Serial.println(" ");
  
  //if (AFTER_CROSSING == _FALSE) {
   
/*
  Serial.print("left_block: ");
  Serial.print(left_block);
  Serial.print(" right_block: ");
  Serial.print(right_block);
  Serial.print(" off_start: ");
  Serial.print(pos);
  Serial.println(" ");
  */
  
  if (((digitalRead(TURN_LEFT_SENSOR) == BLACK_FLAG) && 
       (digitalRead(TURN_RIGHT_SENSOR) == BLACK_FLAG)) &&
       ((digitalRead(LEFT_SENSOR) != BLACK_FLAG) && 
       (digitalRead(RIGHT_SENSOR) != BLACK_FLAG))) {
    left_block = FREE;
    right_block = FREE;
    pos = ON_START;
    if (button_state == SEEKING_START) {
      button_state = WAITING_ON_START;
      fw_2(1);
      return;
    }
    
    if (button_state == RUNNING) {
      button_state = WAITING_ON_START;
      fw_2(1);
      return;
    }
    
    if (button_state == WAITING_ON_START) {
      button_state = WAITING_ON_START;
      fw_2(1);
      return;
    }
  }
  else if (((digitalRead(TURN_LEFT_SENSOR) != BLACK_FLAG) && 
       (digitalRead(TURN_RIGHT_SENSOR) != BLACK_FLAG))) {
    pos = OFF_START;
    if (button_state == WAITING_ON_START) {
      button_state = READY_TO_START;
      fw_2(1);
      delay(50);
      return;
    }
  }
  else {
    if (button_state == WAITING_ON_START) {
      fw_2(1);
      return;
    }
  }
  
  
  if (pos == OFF_START && button_state == RUNNING) {
    if (digitalRead(TURN_LEFT_SENSOR) == BLACK_FLAG) {
      if (right_block == FREE && left_block == FREE) {
        priority = PRIORITY_LEFT;
        right_block = EXPECT_CROSSING_1;
      }
      if (left_block == EXPECT_CROSSING_1) {
        left_block = CROSSING_1;
      }
      if (left_block == EXPECT_CROSSING_2) {
        left_block = CROSSING_2;
      }
    }
    else {
      if (left_block == CROSSING_1) {
        left_block = EXPECT_CROSSING_2;
      }
      if (left_block == CROSSING_2) {
        left_block = FREE;
      }
    }
    
    if (digitalRead(TURN_RIGHT_SENSOR) == BLACK_FLAG) {
      if (right_block == FREE && left_block == FREE) {
        priority = PRIORITY_RIGHT;
        left_block = EXPECT_CROSSING_1;
      }
      if (right_block == EXPECT_CROSSING_1) {
        right_block = CROSSING_1;
      }
      if (right_block == EXPECT_CROSSING_2) {
        right_block = CROSSING_2;
      }
    }
    else {
      if (right_block == CROSSING_1) {
        right_block = EXPECT_CROSSING_2;
      }
      if (right_block == CROSSING_2) {
        right_block = FREE;
      }
    }
  }

  if (button_state == SEEKING_START) {
    if (digitalRead(LEFT_SENSOR) == BLACK_FLAG && priority == PRIORITY_LEFT) {
      left_2(4, 0);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG && priority == PRIORITY_RIGHT) {
      right_2(4, 0);
    }
    else if (digitalRead(LEFT_SENSOR) == BLACK_FLAG) {
      left_2(4, 0);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG) {
      right_2(4, 0);
    } else {
      fw_2(4);
      AFTER_CROSSING = _FALSE;
      split_flag = OFF_SPLIT;
    }
    return;
  }

  
  if (split_flag == OFF_SPLIT) { 
  
    if ((digitalRead(LEFT_SENSOR) == BLACK_FLAG) && (digitalRead(RIGHT_SENSOR) == BLACK_FLAG)) {
        split_flag = ON_SPLIT;
        if ( priority == PRIORITY_LEFT ) {
          left_2(50, 1);
        }
        else {
          right_2(50, 1);
        }
        //AFTER_CROSSING = _TRUE;
        return;
    } else if (digitalRead(LEFT_SENSOR) == BLACK_FLAG) {
        left_2(30, 2);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG) {
        right_2(30, 2);
    } else {
      fw_2(90);
      AFTER_CROSSING = _FALSE;
    }
  
  }
  else {
    if (digitalRead(LEFT_SENSOR) == BLACK_FLAG && priority == PRIORITY_LEFT) {
      left_2(30, 2);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG && priority == PRIORITY_RIGHT) {
      right_2(30, 2);
    }
    else if (digitalRead(LEFT_SENSOR) == BLACK_FLAG) {
      left_2(30, 2);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG) {
      right_2(30, 2);
    } else {
      fw_2(90);
      AFTER_CROSSING = _FALSE;
      split_flag = OFF_SPLIT;
    }
    
  }
  
  // sets the servo position according to the scaled value   
  //delay(15);                           // waits for the servo to get there 
} 
