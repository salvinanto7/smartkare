#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <heartRate.h>
#include <spo2_algorithm.h>
#include<MAX30105.h>
#include<LiquidCrystal.h>
#include<Wire.h>
#include <DS3231.h>
#include <string.h>
#include <Servo.h>

#define speaker 11              //buzzer
DS3231 rtc(SDA, SCL);

char ssid[] = "Redmi Note 7S";     //wifi network name (local wifi used here)
char password[] = "wunderkind";    //wifi password

MAX30105 pulse_sensor;
const byte rate = 5;
byte rate_arr[rate];
byte rate_count=0;
long last_beat=0;
float bpm;
int beat_avg;

// servo function declaration
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


void send_msg(int k);    //k=0 for text msg and k=1 for email and k=2 for med book and  k=3 doc book

LiquidCrystal lcd(2,3,4,5,6,7);

void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting Wifi: ");     //connecting to network
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  if(!pulse_sensor.begin(Wire,I2C_SPEED_FAST)){
    Serial.println("pulse sensor not found");
    while(1);
  }
  else{
    Serial.print("bpm sensor found");
  }
  pulse_sensor.setup();
  pulse_sensor.setPulseAmplitudeRed(0x0A);
  pulse_sensor.setPulseAmplitudeGreen(0);
  lcd.begin(16,2);
  lcd.print("system display good to go");
  myservo1.attach(9);// attaches the servo on pin 9 to the servo object
  myservo2.attach(10);//attaches the servo on pin 10 to the servo object
  myservo3.attach(11);//attaches the servo on pin 11 to the servo object
  myservopup.attach(6);
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
  get_bpm();
  reminder_system();
}

void reminder_system(){
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

void get_bpm(){
  long ir_val=pulse_sensor.getIR();
  if (ir_val>7000){           // finger detected
    lcd.setCursor(0,0);
    lcd.println("Heart rate :");
    lcd.setCursor(0,5);
    lcd.println(beat_avg);
    lcd.println("BPM");
    if(checkForBeat(ir_val)==true){
      long delta=millis()-last_beat;
      last_beat=millis();
      bpm=60/(delta/1000);
      if(bpm<255 && bpm>20){
        rate_arr[rate_count++]=bpm;
        rate_count%=rate;
        beat_avg=0;
        for(byte x=0; x<rate;x++)
          beat_avg+=rate_arr[x];
        beat_avg/=rate;
      }
    }
  }
  if(ir_val<7000){          //no finger detected
    beat_avg=0;
    lcd.setCursor(0,1);
    lcd.println("error reading bpm");
    while(1);
  }
}

void emergency(){
    if (beat_avg>=100 || beat_avg<=58)
    {
        send_msg(1);
        send_msg(0);
    }    
}

void send_msg(int k){
    HTTPClient http;
    if(k=0){                // send text message to fam
        http.begin("https://maker.ifttt.com/trigger/heart_rate/with/key/c5ho6kCRSDWS5XK-pnagHI?value1=1234");
        http.GET();
        http.end();
    }
    else if(k=1){            // send email to fam
        http.begin("https://maker.ifttt.com/trigger/heart-mail/with/key/c5ho6kCRSDWS5XK-pnagHI?value1=1234");
        http.GET();
        http.end();
    }
    
}

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
    for (pos2 = 0; pos2 <= 90; pos2 += 1) { 
    // in steps of 1 degree
    myservo1.write(pos1);              
    delay(15);                       
  }
  for (pos2 = 90; pos2 >= 0; pos2 -= 1) { 
    myservo2.write(pos2);              
    delay(15); 
    }
}
void medi3 (){
    for (pos3 = 0; pos3 <= 90; pos3 += 1) { 
    // in steps of 1 degree
    myservo3.write(pos3);              
    delay(15);                       
  }
  for (pos3 = 90; pos3 >= 0; pos3 -= 1) { 
    myservo3.write(pos3);              
    delay(15); 
    }
}
void tray (){
    for (pospup = 0; pospup <= 180; pospup += 1) { 
    // in steps of 1 degree
    myservopup.write(pospup);              
    delay(120000);                       
  for (pospup = 180; pospup >= 0; pospup -= 1) { 
    myservopup.write(pospup);              
    delay(100); 
    }
}
