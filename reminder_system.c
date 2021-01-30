#include <DS3231.h>
#include <string.h>

#define speaker 11              //buzzer
DS3231 rtc(SDA, SCL);

void setup(){
  rtc.setDOW(SATURDAY);
  rtc.setTime(16, 0, 0);
  rtc.setDate(01, 30, 2021);
  rtc.getDateStr();
  String Time;
  String time_arr;
  int med_flag=1;
  int count=0;
}
void loop(){
 delay(30000);  //at present 30 sec gap between each call
 Time=rtc.getTimeStr();
  for(int i=0;i<5;i++){
    time_arr[i]=Time[i];
  }
  if(strcmp(time_arr,"09:00")==0){            //breakfast
    alarm();
  }
  else if(strcmp(time_arr,"12:00")==0){       //lunch
    alarm();
  }
  else if(strcmp(time_arr,"19:30")==0){       //dinner
    alarm();
  }
  else{
    medicine();                              // check if it is med time
  }
}

/*void breakfast(){ 
  Time=rtc.getTimeStr();
  for(int i=0;i<5;i++){
    time_arr[i]=Time[i];
  }
  if(strcmp(time_arr,"09:00")==0){
    alarm();
  }
}

void lunch(){
  Time=rtc.getTimeStr();
  for(int i=0;i<5;i++){
    time_arr[i]=Time[i];
  }
  if(strcmp(time_arr,"12:00")==0){
    alarm();
  }
}

void dinner(){
  Time=rtc.getTimeStr();
  for(int i=0;i<5;i++){
    time_arr[i]=Time[i];
  }
  if(strcmp(time_arr,"19:30")==0){
    alarm();
  }
}*/

void medicine(){
  Time=rtc.getTimeStr();
  for(int i=0;i<5;i++){
    time_arr[i]=Time[i];
  }
  if(strcmp(time_arr,"08:30")==0){          //got to take a med at 8.30am
    alarm();
    med_box(mb);                            
  }
  else if(strcmp(time_arr,"10:00")==0 && med_flag ==1 ){
    alarm(); 
    med_box(ma);   // pills after breakfast
  }
  else if(strcmp(time_arr,"13:00")==0 && med_flag==1){
    alarm();
    med_box(la);    //pills after lunch
  }
  else if(strcmp(time_arr,"19:00")==0){
    alarm();
    med_box(db);    //pills before dinner
  }
  else if(strcmp(time_arr,"20:00"==0 && med_flag==1){
    alarm();
    med_box(da);     //pills after dinner
  }
}
void alarm(){             // need to add flags to show which msg to be send to fam
  while(pressure!=0){    // pressure is the var for the preessure sensor bvelow the plate
    pinMode(speaker,HIGH);
    delay(300000);
    pinMode(speaker,LOW);
    count++;
    if(count==5{
      send_msg();             //send a text msg to CARE TAKER
      break;
    }
  }
  if(count==0){
    med_flag=1;
  }
  else{
   med_flag=0;  
  }
}

void med_box(String t){
    if(t=="mb"){
        medi1();
        delay(100);
        tray();
    }
    else if(t=="ma")
    {
        medi2();
        medi3();
        delay(100);
        tray();
    }
    else if (t=="la")
    {
        medi2();
        delay(100);
        tray();
    }
    else if (t=="db")
    {
        medi1();
        delay(100);
        tray();
    }
    else if (t=="da")
    {
        medi2();
        medi3();
        delay(100);
        tray();
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
    delay(120000);                       // waits 15ms for the servo to reach the position
  }
  for (pospup = 180; pospup >= 0; pospup -= 1) { // goes from 180 degrees to 0 degrees
    myservopup.write(pospup);              // tell servo to go to position in variable 'pos'
    delay(100); 
    }
}
