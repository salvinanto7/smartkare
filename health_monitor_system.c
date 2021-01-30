#include <heartRate.h>
#include <spo2_algorithm.h>
#include<MAX30105.h>
#include<LiquidCrystal.h>
#include<Wire.h>

MAX30105 pulse_sensor;
const byte rate = 5;
byte rate_arr[rate];
byte rate_count=0;
long last_beat=0;
float bpm;
int beat_avg;

LiquidCrystal lcd(2,3,4,5,6,7);

void setup(){
  Serial.begin(9600);
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
}

void loop(){
  long ir_val=pulse_sensor.getIR();

  if (ir_val>7000){           // finger detected
    if(checkForBeat(ir_val)==true){
      lcd.setCursor(0,0);
      lcd.println("Heart rate :");
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
