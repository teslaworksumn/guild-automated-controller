#include <string.h>
#include <ctype.h>
#include <QueueList.h>

// define a constant value named stepPin and assign the value 9 to it - this value will not change during our code
// this assumes digital pin 9 of your Arduino is attached to the step input of your driver
#define stepPinOne 7
#define stepPinTwo 5
#define stepPinThree 3
 
// define a constant value named dirPin and assign the value 8 to it - this value will not change during our code
// this assumes digital pin 8 of your Arduino is attached to the step input of your driver
#define dirPinOne 6
#define dirPinTwo 4
#define dirPinThree 2

#define home1pin 10

#define home2pin 9
#define home3pin 8

#define min1 60.0
#define max1 144.0
#define min2 65.0
#define max2 106.0
#define min3 110.0
#define max3 153.0
#define home1 144.0
#define home2 106.0
#define home3 153.0
#define angle_per_16_step 0.0275
#define unit_angle 0.0275

#define stepDelay 200
#define movestep 10

boolean stateComplete = false;  // whether the string is complete
float curr1, curr2, curr3, new1, new2, new3; // current and target angles for each motor
QueueList <float> angle1_q;
QueueList <float> angle2_q;
QueueList <float> angle3_q;

String inputString="";
int stringComplete;

int axis=0;
int dir = -1; //used to switch direction
float error = angle_per_16_step*movestep/2;

void setup() {
  pinMode(stepPinOne , OUTPUT);
  pinMode(dirPinOne , OUTPUT);
  pinMode(stepPinTwo, OUTPUT);
  pinMode(dirPinTwo, OUTPUT);
  pinMode(stepPinThree, OUTPUT);
  pinMode(dirPinThree, OUTPUT);
 
  pinMode(home1pin, INPUT);
  pinMode(home2pin, INPUT);
  pinMode(home3pin, INPUT);
  
  Serial.begin(9600);
  
  inputString.reserve(200);
  
  goHome();
  curr1 = home1;
  curr2 = home2;
  curr3 = home3;
  //Serial.println("First\n");
}

void loop() {
  boolean changed = true;
  int steps;
  
  if (!angle1_q.isEmpty() && !angle2_q.isEmpty() && !angle3_q.isEmpty()) {
//    if(!angle1_q.isEmpty() && !angle2_q.isEmpty() && !angle3_q.isEmpty())
//    {
    new1 = angle1_q.pop();
    new2 = angle2_q.pop();
    new3 = angle3_q.pop();
//    }
//    else
//    {
//      new1 = home1;
//      new2 = home2;
//      new3 = home3;
//    }
    
    if(new1>max1){
      new1=max1;
    }
    else if(new1<min1){
      new1=min1;
    }
    if(new2>max2){
      new2=max2;
    }
    else if(new2<min2){
      new2=min2;
    }
    if(new3>max3){
      new3=max3;
    }
    else if(new3<min3){
      new3=min3;
    }

    if( new1 == 0 && new2 == 0 && new3 == 0 )
    {
          goHome();
          curr1 = home1;
          curr2 = home2;
          curr3 = home3;
    }
    else 
    {
      steps = (curr1-new1)/unit_angle;
      curr1-=steps*unit_angle;
      goStep(dir*steps,1);
      
      steps = (curr2-new2)/unit_angle;
      curr2-=steps*unit_angle;
      goStep(-1*dir*steps,2);
      
      steps = (curr3-new3)/unit_angle;
      curr3-=steps*unit_angle;
      goStep(dir*steps,3);
    }
   
  }
  stateComplete = false;
  delay(500);
  Serial.print("0");
}

void serialEvent(){
  char inChar = -1;
  String s = "";
  int count=1;
  char tp[50];
  while (Serial.available()) {
      angle1_q.push(Serial.parseFloat());
      angle2_q.push(Serial.parseFloat());
      angle3_q.push(Serial.parseFloat());
      stateComplete = true;

  }

    
     
//    Serial.println(s);
//    new1 = 1.0;
//    new2 = 2.0;
//    new3 = 3.0;
    
//    new1 = Serial.parseFloat();
//    new2 = Serial.parseFloat();
//    new3 = Serial.parseFloat();
//    
//    Serial.print("new1 ");
//    Serial.println(new1);
//    Serial.print("new2 ");
//    Serial.println(new2);
//    Serial.print("new3 ");
//    Serial.println(new3);
//    
    
   
}

void goHome(){
  while(digitalRead(home1pin)==HIGH | digitalRead(home2pin)==HIGH | digitalRead(home3pin)==HIGH){
    if(digitalRead(home1pin)==HIGH){
      goStep(5,1);
    }
    if(digitalRead(home2pin)==HIGH){
      goStep(-5,2);
    }       
    if(digitalRead(home3pin)==HIGH){
      goStep(5,3);
    }
  }
}

void goStep(int nStep, int axis){
  //Serial.println(axis);
  if(axis==1){
        if(nStep > 0){
            digitalWrite(dirPinOne, HIGH);
            accelStep(nStep,stepPinOne);
        } else if(nStep < 0){
            digitalWrite(dirPinOne, LOW);
            accelStep(nStep*-1,stepPinOne);
        }
    }else if(axis==2){
        if(nStep > 0){
            digitalWrite(dirPinTwo, HIGH);
            accelStep(nStep,stepPinTwo);
        } else if(nStep < 0){
            digitalWrite(dirPinTwo, LOW);
            accelStep(nStep*-1,stepPinTwo);
        }
    }else if(axis==3){
        if(nStep > 0){
            digitalWrite(dirPinThree, HIGH);
            accelStep(nStep,stepPinThree);
        } else if(nStep < 0){
            digitalWrite(dirPinThree, LOW);
            accelStep(nStep*-1,stepPinThree);
        }
    }
}

void accelStep(int nStep, int stepPin){
  for(int i=0; i<nStep; i++){
    digitalWrite(stepPin , HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin , LOW);
    delayMicroseconds(stepDelay);
  }
}
