#include <servo.h>
#include <TimerOne.h>
#include <RTClib.h>

servo myservo1;
servo myservo2;
servo myservo3;
servo myservopopup;

int pos1, pos2, pos3, pospopup, time;
int hour, minute, fsr=10, speaker=5;
int med1_count, med2_count, med3_count;
int date, appoint_flag=0;
int am_food_alarm, noon_food_alarm, pm_food_alarm;

RTC_DS1307 rtc;

void setup(){
    myservo.attach(12);
    myservo.attach(13);
    myservo.attach(14);
    myservo.attach(15);
    Timer1.initialize(60000);
    Timer1.attachInterrupt(timecheck());
    rtc_init();
    pinMode(fsr,INPUT);  // FSR INPUT
    Serial.print("Spare a few minutes to calibrate the medicine count");
    Serial.print("Enter the number of medicine in tray1");
    med1_count=Serial.read();
    Serial.print("Enter the number of medicine in tray2");
    med2_count=Serial.read();
    Serial.print("Enter the number of medicine in tray3");
    med3_count=Serial.read();
    Serial.print("Would you like to set an alarm for the food? type in yes or no");
    if(Serial.read()=="yes"){
        Serial.print("Do you want an alarm for breakfast? type 1 for yes and 0 for no");
        am_food_alarm=Serial.read();
        Serial.print("Do you want an alarm for lunch? type 1 for yes and 0 for no");
        noon_food_alarm=Serial.read();
        Serial.print("Do you want an alarm for dinner? type 1 for yes and 0 for no");
        pm_food_alarm=Serial.read();
    }
    Serial.print("system starting now...");
}

void loop(){
    medicine_booking();
    appointment();
}

void rtc_init(){
    if (! rtc.begin()){
        Serial.print("Couldn't find RTC");
        while (1);
  }
    if (! rtc.isrunning()){
        Serial.print("RTC is NOT running!");
  }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // auto udate from pc date and time
}

void timecheck(){
    DateTime now=rtc.now;
    hour=now.hour();
    minute=now.minute();

    if(hour==8 && minute==15 && am_food_flag==1){
        food_alarm();
        am_food_flag=0;
    }
    if(hour==12 && minute==0 && noon_food_flag==1){
        food_alarm();
        noon_food_flag=0;
    }
    if(hour==19 && minute==15 && pm_food_flag==1){
        food_alarm();
        pm_food_flag=0;
    }
    if(hour==9 && minute==0){
        medi_am();
    }
    else if(hour==12 && minute==30){
        medi_noon();
    }
    else if(hour==20 && minute==0){
        medi_pm();
    }
}

void medi_am(){   // assuming that 2 medicines in box 1 and 2 are to be dispenced
    for(pos1=0;pos1<=90;pos1++){
        myservo1.write(pos1);
    }
    delay(500);
    for(pos1=90;pos1>=0;pos1--){
        myservo1.write(pos1);
    }
    med1_count--;
    for(pos2=0;pos2<=90;pos2++){
        myservo2.write(pos2);
    }
    delay(500);
    for(pos2=90;pos2>=0;pos2--){
        myservo2.write(pos2);
    }
    med2_count--;
    tray();
}

void medi_noon(){    //assuming only medicine from box 3 is necessary
    for(pos3=0;pos3<=90;pos3++){
        myservo3.write(pos3);
    }
    delay(500);
    for(pos3=90;pos3>=0;pos3--){
        myservo3.write(pos3);
    }
    med3_count--;
    tray();
}

void medi_pm(){         //assuming medicine from box 1 and 3 are to be dispenced
    for(pos1=0;pos1<=90;pos1++){
        myservo1.write(pos1);
    }
    delay(500);
    for(pos1=90;pos1>=0;pos1--){
        myservo1.write(pos1);
    }
    med1_count--;
    for(pos3=0;pos3<=90;pos3++){
        myservo3.write(pos3);
    }
    delay(500);
    for(pos3=90;pos3>=0;pos3--){
        myservo3.write(pos3);
    }
    med3_count--;
    tray();
}

void tray(){
    for(pospopup=0;pospopup<=180;pospopup++){
        myservopopup.write(pospopup);
    }
    while (fsr!=0);                 //waits open until the medicine is taken from the tray
    for(pospopup=180;pospopup>=0;pospopup--){
        myservopopup.write(pospopup);
    }    
}

void medicine_booking(){
  if(med1_count>5 && med1_count<7){
        http.begin("https://maker.ifttt.com/trigger/med_stock/with/key/c5ho6kCRSDWS5XK-pnagHI?value1=1234");
        http.GET();
        http.end();
  }
  if(med2_count>5&&med2_count<7){
        http.begin("https://maker.ifttt.com/trigger/med_stock/with/key/c5ho6kCRSDWS5XK-pnagHI?value1=1234");
        http.GET();
        http.end();
  }
  if(med3_count>5&&med3_count<7){
        http.begin("https://maker.ifttt.com/trigger/med_stock/with/key/c5ho6kCRSDWS5XK-pnagHI?value1=1234");
        http.GET();
        http.end();
  }
}

void appointment(){
    date=now.day;
    if(date==10 && appoint_flag==0){
        http.begin("https://maker.ifttt.com/trigger/med_stock/with/key/c5ho6kCRSDWS5XK-pnagHI?value1=1234");
        http.GET();
        http.end();
    }
    appoint_flag=1;
}