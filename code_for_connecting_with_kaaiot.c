#include <kaa.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const char* ssid = "Redmi Note 7S";        // WiFi name
const char* password = "wunderkind";    // WiFi password
const char* mqtt_server = "mqtt.cloud.kaaiot.com";
const String TOKEN = "smartkare";        // Endpoint token 
const String APP_VERSION = "c1isciarqa51r8l3rh9g-v1";  // Application version

const unsigned long fiveSeconds = 1 * 5 * 1000UL;
static unsigned long lastPublish = 0 - fiveSeconds;
int reconnect_count=0;

WiFiClient espClient;
PubSubClient client(espClient);

MAX30105 pulse_sensor;
int rate = 5;
float rate_arr[rate];
float rate_count=0;
long last_beat=0;
float bpm;
float beat_avg;

TinyGPSPlus gps;
SoftwareSerial gps_mod(3,1);
float latitude=0.0, longitude=0.0;
int hour=00, minute=00, second=00;

void setup() {
  Serial.begin(115200);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  bpm_setup();
  gps_mod.begin();
}

void loop() {
  setup_wifi();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  gethealth();
  unsigned long now = millis();
  if (now - lastPublish >= fiveSeconds) {
    lastPublish += fiveSeconds;
    DynamicJsonDocument telemetry(1023);
    telemetry.createNestedObject();                             // need to edit
    telemetry[0]["heart rate"] = beat_avg;
    telemetry[0]["systole"] = random(70, 95);
    telemetry[0]["diastole"]=random(100, 140);

    String topic = "kp1/" + APP_VERSION + "/dcx/" + TOKEN + "/json";
    client.publish(topic.c_str(), telemetry.as<String>().c_str());
    Serial.println("Published on topic: " + topic);
  }
  emergency();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.printf("\nHandling command message on topic: %s\n", topic);

  DynamicJsonDocument doc(1023);
  deserializeJson(doc, payload, length);
  JsonVariant json_var = doc.as<JsonVariant>();

  DynamicJsonDocument commandResponse(1023);
  for (int i = 0; i < json_var.size(); i++) {
    unsigned int command_id = json_var[i]["id"].as<unsigned int>();
    commandResponse.createNestedObject();
    commandResponse[i]["id"] = command_id;
    commandResponse[i]["statusCode"] = 200;
    commandResponse[i]["payload"] = "done";
  }

  String responseTopic = "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/result/SWITCH";
  client.publish(responseTopic.c_str(), commandResponse.as<String>().c_str());
  Serial.println("Published response to SWITCH command on topic: " + responseTopic);
}

void setup_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.println();
    Serial.printf("Connecting to [%s]", ssid);
    WiFi.begin(ssid, password);
    connectWiFi();
  }
}

void connectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() { 
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    char *client_id = "client-id-123ab";
    if (client.connect(client_id)) {
      Serial.println("Connected to WiFi");
      // ... and resubscribe
      subscribeToCommand();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      reconnect_count++; 
      if (reconnect_count>=4){
          Serial.println("person missing from wifi range");
          send_msg(2);   // inform fam members that the person is missing from the premices alng with the current location
      }
    }
  }
}

void subscribeToCommand() {
  String topic = "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/command/SWITCH/status";
  client.subscribe(topic.c_str());
  Serial.println("Subscribed on topic: " + topic);
}

void bpm_setup(){
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
}
void gethealth(){
    // get bpm
    long ir_val=pusle_sensor.getIR();

    if(ir_val>7000){
        if(checkForBeat(ir_val)==true){
            long delta=millis()-last_beat;
            last_beat=millis();
            bpm=60/(delta/1000);
            if (bpm>20 && bpm<255)
            {
                rate_arr[rate_count++]=bpm;
                rate_count%=rate;
                beat_avg=0;
                for(int x=0;x<rate;x++){
                    beat_avg+=ratearr[x];
                }
                beat_avg/=rate;
            }
            
        }
    else{          //no finger detected
    beat_avg=0;
    Serial.println("no finger detected. Try again"); //debugg purose
    while(1);
  }
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
    if(k=0){                // send text message to family
        http.begin("https://maker.ifttt.com/trigger/heart_rate/with/key/c5ho6kCRSDWS5XK-pnagHI?value1=1234");
        http.GET();
        http.end();
    }
    else if(k=1){        // send an email for medical appointment 
        http.begin("https://maker.ifttt.com/trigger/heart-mail/with/key/c5ho6kCRSDWS5XK-pnagHI?value1=1234");    // make necessary changes in applets
        http.GET();
        http.end();
    }
    else if(k=2)  {            // notification to family members that the person is missing from wifi range along with the current location
        getloaction();
        http.begin("");
        http.GET();
        http.end();
    }
}

int getlocation(){
    if (gps_mod.available()>0){
        Serial.print("gps available");
        if (gps.encode(gps_mod.read())){
            Serial.print("Gps reading data");
            if(gps.location.isValid()){
                Serial.print("location available");
                latitude=gps.location.lat();
                longitude=gps.location.lng();
            }
            if (gps.date.isValid()){
                Serial.print("date available");
                hour=gsp.time.hour();
                hour+=5;                    // conversion to IST
                minute=gps.time.minute();
                minute+=30;
                second=gpd.time.second();
            }
        }
    }
    return latitude, longitude, hour, minute, second;
}