#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3; 
Servo myservopup; 

// create servo object to control a servo
// twelve servo objects can be created on most boards

int pos1 = 0;    // variable to store the servo position
int pos2 = 0;
int pos3 = 0;
int pospup = 0;
int time;       // variable to store time
void setup() {
  myservo1.attach(9);// attaches the servo on pin 9 to the servo object
  myservo2.attach(10);//attaches the servo on pin 10 to the servo object
  myservo3.attach(11);//attaches the servo on pin 11 to the servo object
  myservopup.attach(6);
}

void loop() {
  if(time==9){
 
  medi1;
  delay(500);
  tray;
    
    }

  
  if(time==12){
           medi1;
           medi2;
           medi3; 
                 delay(500);
                tray;     
  }

  if(time==21){
    
          medi3;
           delay(500);
           tray;
    }
  
  }

void medi1 (){
    for (pos1 = 0; pos1 <= 90; pos1 += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos1 = 90; pos1 >= 0; pos1 -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15); 
    }
}
void medi2 (){
    for (pos2 = 0; pos2 <= 90; pos2 += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos2 = 90; pos2 >= 0; pos2 -= 1) { // goes from 180 degrees to 0 degrees
    myservo2.write(pos2);              // tell servo to go to position in variable 'pos'
    delay(15); 
    }
}
void medi3 (){
    for (pos3 = 0; pos3 <= 90; pos3 += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo3.write(pos3);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos3 = 90; pos3 >= 0; pos3 -= 1) { // goes from 180 degrees to 0 degrees
    myservo3.write(pos3);              // tell servo to go to position in variable 'pos'
    delay(15); 
    }
}
void tray (){
    for (pospup = 0; pospup <= 180; pospup += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservopup.write(pospup);              // tell servo to go to position in variable 'pos'
    delay(2500);                       // waits 15ms for the servo to reach the position
  }
  for (pospup = 180; pospup >= 0; pospup -= 1) { // goes from 180 degrees to 0 degrees
    myservopup.write(pospup);              // tell servo to go to position in variable 'pos'
    delay(100); 
    }
}
