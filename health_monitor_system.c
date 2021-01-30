#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <heartRate.h>
#include <spo2_algorithm.h>
#include<MAX30105.h>
#include<LiquidCrystal.h>
#include<Wire.h>

char ssid[] = "Redmi Note 7S";     //wifi network name (local wifi used here)
char password[] = "wunderkind";    //wifi password

MAX30105 pulse_sensor;
const byte rate = 5;
byte rate_arr[rate];
byte rate_count=0;
long last_beat=0;
float bpm;
int beat_avg;

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
}

void loop(){
  get_bpm();
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