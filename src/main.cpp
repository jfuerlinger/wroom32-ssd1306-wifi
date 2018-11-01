/*
  HiGrowESP32MQTT
  (c) Claus Kuehnel 2018-03-18 info@ckuehnel.ch
  The ESP32 reads data from HiGrow device and publishes these via MQTT
  based on https://github.com/LilyGO/higrowopen/tree/master/HiGrowEsp32
*/

#include <WiFi.h>

#include <PubSubClient.h>
#include "DHT.h"
#include "credentials.h"

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define uS_TO_S_FACTOR 1000000

//int DEEPSLEEP_SECONDS = 1800; // 30 min
int DEEPSLEEP_SECONDS = 180; // 3 min

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

uint64_t chipid;

const int dhtpin = 22;
const int soilpin = 32;
const int POWER_PIN = 34;
const int LIGHT_PIN = 33;

// Initialize DHT sensor.
DHT dht(dhtpin, DHTTYPE);

// Temporary variables
char msg[20];

/* topics */
#define TEMP_TOPIC    "HiGrow/temp"
#define HUMI_TOPIC    "HiGrow/humi"
#define SOIL_TOPIC    "HiGrow/soil"
#define LIGHT_TOPIC   "HiGrow/light"

// Client variables 
char linebuf[80];
int charcount=0;

char deviceid[21];

void receivedCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
  Serial.println();
}


void configureMQTT()
{
  /* configure the MQTT server with IPaddress and port */
  client.setServer(BROKER, BRPORT);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);
}


void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    
    /* connect now */
    if (client.connect(CLIENTID, BRUSER, BRPWD)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}

void connectWiFi()
{
  int count = 0;
  Serial.print("Connecting to ");
  Serial.println(mySSID);
  WiFi.begin(mySSID, myPW);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    count++;
    if (count > 15) ESP.restart();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sensorsData(char* body)
{
  //This section reads all sensors
  
  int waterlevel = analogRead(soilpin);
  int lightlevel = analogRead(LIGHT_PIN);
  
  waterlevel = map(waterlevel, 0, 4095, 0, 1023);
  waterlevel = constrain(waterlevel, 0, 1023);
  if (!isnan(waterlevel)) 
  {
    snprintf (msg, 20, "%d", waterlevel);
    /* publish the message */
    client.publish(SOIL_TOPIC, msg);
  }
  
  lightlevel = map(lightlevel, 0, 4095, 0, 1023);
  lightlevel = constrain(lightlevel, 0, 1023);
  if (!isnan(lightlevel)) 
  {
    snprintf (msg, 20, "%d", lightlevel);
    /* publish the message */
    client.publish(LIGHT_TOPIC, msg);
  }
  
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  if (!isnan(humidity)) 
  {
    snprintf (msg, 20, "%5.1f", humidity);
    /* publish the message */
    client.publish(HUMI_TOPIC, msg);
  }
  
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  if (!isnan(temperature)) 
  {
    snprintf (msg, 20, "%5.1f", temperature);
    /* publish the message */
    client.publish(TEMP_TOPIC, msg);
  }
 
  Serial.print("DevideId: "); Serial.println(deviceid);
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" *C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %rF");
  Serial.print("Soil: "); Serial.println(waterlevel);
  Serial.print("Light: "); Serial.println(lightlevel);
}


void setup() 
{
  dht.begin();
  
  Serial.begin(115200);
  delay(2000); // wait for monitor

  esp_sleep_enable_timer_wakeup(DEEPSLEEP_SECONDS * uS_TO_S_FACTOR);

  pinMode(16, OUTPUT); // blue LED
  pinMode(POWER_PIN, INPUT);
  digitalWrite(16, LOW);  

  chipid = ESP.getEfuseMac();
  sprintf(deviceid, "%" PRIu64, chipid);
  Serial.print("DeviceId: ");
  Serial.println(deviceid);

  connectWiFi();
  configureMQTT();
}





void loop() 
{
  char body[1024];
  digitalWrite(16, LOW); //switched on

  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }

  sensorsData(body);
  delay(500);
  //WiFi.disconnect(true);
  //Serial.println("Going to Deep Sleep..."); // esp_deep_sleep_start();    // uncomment for deep sleep
  delay(5000);               // used for test
}

