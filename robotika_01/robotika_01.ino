
// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
// serva 
Servo myservoL;  // create servo object to control a servo 
Servo myservoR; //  // create servo object to control a servo 
 
// asi zbytecne - z examplu 
int potpin = 0;  // analog pin used to connecti the potentiometer
int val;    // variable to read the value from the analog pin 

//int sensors[] = {11, 9, 10};
//int sensorVals[] = {0, 0, 0};

//int sensors[] = {7, 6, 5, 4, 3};

int sensors[] = {3, 4, 5, 6, 7};  // piny senzoru
int sensorVals[] = {0, 0, 0, 0, 0};   // nasnimane hodnoty senzoru  
const int BUTTON_SENSOR = 8;    // pin tlacitka

//int sensorCount = 3;
int sensorCount = 5;    // pocet senzoru

//int LEFT_SENSOR = sensors[0];
//int CENTER_SENSOR = sensors[1];
//int RIGHT_SENSOR = sensors[2];

// pojmenovane jednotlive senzory
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
int BLACK_FLAG = 0;  // hodnota cary
 
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

// pohyb vpred 
void fw(int sp) {
  myservoL.write(90 - sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 + sp);                  // sets the servo position according to the scaled value   

} 

void fw_2(int sp) {
  myservoL.write(90 + sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 - sp);                  // sets the servo position according to the scaled value   

} 

// otaceni doleva 
void left(int sp) {
  myservoL.write(90 - sp);                  // sets the servo position according to the scaled value 
  myservoR.write(90 - (sp/2));                  // sets the servo position according to the scaled value   

} 
// doleva s otacenim druheho kola
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

// priority, kam zatacet
int PRIORITY_LEFT = 0xFEEDDEEF;
int PRIORITY_RIGHT = 0xFEEDBEEF;
int PRIORITY_NONE = 0xDEADBEEF;

// priorita, kam zatacet
int priority = PRIORITY_RIGHT;

int left_block = 0;
int right_block = 0;

// stav kde jsme
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

// stavy tlacitka
int WAITING_ON_PUSH = 255;
int SEEKING_START = 0;
int WAITING_ON_START = 1;
int READY_TO_START = 2;
int RUNNING = 3;
int FINISHED = 4;
int button_state = WAITING_ON_PUSH;

// kam jsme zataceli naposled - v pripade, ze uplne ztratime caru
const int LAST_TURN_LEFT = 0;
const int LAST_TURN_RIGHT = 1;
const int LAST_TURN_NONE = 2;
int lastTurn = LAST_TURN_NONE;


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
      if (right_block == FREE && left_block == FREE && digitalRead(CENTER_SENSOR) == BLACK_FLAG) {
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
      if (right_block == FREE && left_block == FREE && digitalRead(CENTER_SENSOR) == BLACK_FLAG) {
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
    /*if (digitalRead(LEFT_SENSOR) == BLACK_FLAG && priority == PRIORITY_LEFT) {
      left_2(60, -5);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG && priority == PRIORITY_RIGHT) {
      right_2(60, -5);
    }
    else*/ if (digitalRead(LEFT_SENSOR) == BLACK_FLAG) {
      left_2(20, 5);
      //delay(100);
    } else /*if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG)*/ {
      right_2(20, 5);
      //delay(100);
    } /*else {
      fw_2(10);
      split_flag = OFF_SPLIT;
    }*/
    return;
  }

  if (digitalRead(LEFT_SENSOR) == BLACK_FLAG) {
      lastTurn = LAST_TURN_LEFT;
  } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG) {
      lastTurn = LAST_TURN_RIGHT;
  } else if (digitalRead(CENTER_SENSOR) == BLACK_FLAG) {
      lastTurn = LAST_TURN_NONE;
  }

  if (split_flag == OFF_SPLIT) { //na ceste
    
    if ((right_block == EXPECT_CROSSING_1 || left_block == EXPECT_CROSSING_1) && digitalRead(LEFT_SENSOR) == BLACK_FLAG && digitalRead(CENTER_SENSOR) == BLACK_FLAG && priority == PRIORITY_RIGHT) {
      fw_2(90);
    } else if ((right_block == EXPECT_CROSSING_1 || left_block == EXPECT_CROSSING_1) && digitalRead(RIGHT_SENSOR) == BLACK_FLAG && digitalRead(CENTER_SENSOR) == BLACK_FLAG && priority == PRIORITY_LEFT) {
      fw_2(90);
    } else
    if ((right_block == EXPECT_CROSSING_2 || left_block == EXPECT_CROSSING_2) && digitalRead(LEFT_SENSOR) == BLACK_FLAG && digitalRead(CENTER_SENSOR) == BLACK_FLAG && priority == PRIORITY_RIGHT) {
      fw_2(90);
    } else if ((right_block == EXPECT_CROSSING_2 || left_block == EXPECT_CROSSING_2) && digitalRead(RIGHT_SENSOR) == BLACK_FLAG && digitalRead(CENTER_SENSOR) == BLACK_FLAG && priority == PRIORITY_LEFT) {
      fw_2(90);
    }  
    else if ((digitalRead(LEFT_SENSOR) == BLACK_FLAG) && (digitalRead(RIGHT_SENSOR) == BLACK_FLAG)) {
        split_flag = ON_SPLIT;
        if ( priority == PRIORITY_LEFT ) {
          left_2(50, 1);
        }
        else {
          right_2(50, 1);
        }
        return;
    } else if (digitalRead(LEFT_SENSOR) == BLACK_FLAG) {
        left_2(30, 0);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG) {
        right_2(30, 0);
    } else if (digitalRead(RIGHT_SENSOR) != BLACK_FLAG && digitalRead(LEFT_SENSOR) != BLACK_FLAG && digitalRead(CENTER_SENSOR) != BLACK_FLAG) {
      if (lastTurn == LAST_TURN_LEFT) {
        left_2(30, 2);
      } else if (lastTurn == LAST_TURN_RIGHT) {
        right_2(30, 2);
      } else {
        fw_2(90);
      }
    } else
    {
      fw_2(90);
    }
  
  }
  else {
    if (digitalRead(LEFT_SENSOR) == BLACK_FLAG && priority == PRIORITY_LEFT) {
      left_2(30, 2);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG && priority == PRIORITY_RIGHT) {
      right_2(30, 2);
    } else if ((right_block == EXPECT_CROSSING_1 || left_block == EXPECT_CROSSING_1) && digitalRead(LEFT_SENSOR) == BLACK_FLAG && digitalRead(CENTER_SENSOR) == BLACK_FLAG && priority == PRIORITY_RIGHT) {
      fw_2(90);
    } else if ((right_block == EXPECT_CROSSING_1 || left_block == EXPECT_CROSSING_1) && digitalRead(RIGHT_SENSOR) == BLACK_FLAG && digitalRead(CENTER_SENSOR) == BLACK_FLAG && priority == PRIORITY_LEFT) {
      fw_2(90);
    }
    else if ((right_block == EXPECT_CROSSING_2 || left_block == EXPECT_CROSSING_2) && digitalRead(LEFT_SENSOR) == BLACK_FLAG && digitalRead(CENTER_SENSOR) == BLACK_FLAG && priority == PRIORITY_RIGHT) {
      fw_2(90);
    } else if ((right_block == EXPECT_CROSSING_2 || left_block == EXPECT_CROSSING_2) && digitalRead(RIGHT_SENSOR) == BLACK_FLAG && digitalRead(CENTER_SENSOR) == BLACK_FLAG && priority == PRIORITY_LEFT) {
      fw_2(90);
    }
    else if (digitalRead(LEFT_SENSOR) == BLACK_FLAG) {
      left_2(30, 2);
    } else if (digitalRead(RIGHT_SENSOR) == BLACK_FLAG) {
      right_2(30, 2);
    } else {
      fw_2(90);
      split_flag = OFF_SPLIT;
    }
    
  }
  
  // sets the servo position according to the scaled value   
  //delay(15);                           // waits for the servo to get there 
} 
