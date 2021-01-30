#include <DS3231.h>
#include <string.h>

#define speaker 11
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

void breakfast(){ 
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
}

void medicine(){
  Time=rtc.getTimeStr();
  for(int i=0;i<5;i++){
    time_arr[i]=Time[i];
  }
  if(strcmp(time_arr,"08:30")==0){          //got to take a med at 8.30am
    alarm();
  }
  else if(strcmp(time_arr,"10:00")==0 && med_flag ==1 ){
    alarm(); 
    med_box(ab);   // pills after breakfast
  }
  else if(strcmp(time_arr,"13:00")==0 && med_flag==1){
    alarm();
    med_box(al);    //pills after lunch
  }
  else if(strcmp(time_arr,"19:00")==0){
    alarm();
    med_box(bd);    //pills before dinner
  }
  else if(strcmp(time_arr,"20:00"==0 && med_flag==1){
    alarm();
    med_box(ad);     //pills after dinner
  }
}
void alarm(){             // need to add flags to show which msg to be send to fam
  while(pressure!=0){    // pressure is the var for the preessure sensor bvelow the plate
    pinMode(speaker,HIGH);
    delay(300000);
    pinMode(speaker,LOW);
    count++;
    if(count==5){
      send_msg();             //send a text msg to family members
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
