#include <WiFi.h>
#include <PubSubClient.h>
/************ WIFI and MQTT INFORMATION (CHANGE THESE FOR YOUR SETUP) ******************/
#define WIFI_SSID "" //type your WIFI information inside the quotes
#define WIFI_PASSWORD ""

#define MQTT_SERVER "192.168.1.1"
#define MQTT_USER "" 
#define MQTT_PASSWORD ""
#define MQTT_PORT 1883

#define SENSORNAME "RCWL-0516-Motion-1"

/**************************** PIN DEFINITIONS ********************************************/
#define SENSOR_PIN 34
#define ONBOARD_LED  2

boolean flag = false;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(ONBOARD_LED, OUTPUT);
  Serial.begin(115200);
  delay(10);
  setup_wifi();
  setup_mqtt();
  connect_mqtt();
  Serial.println("Waiting for motion");
}


/********************************** START MAIN LOOP***************************************/
void loop() {
  boolean movementSensed = digitalRead(SENSOR_PIN);
  
  if((movementSensed) && (!flag))
  {
    digitalWrite(ONBOARD_LED, HIGH);
    Serial.println("Motion Detected");
    flag = true;
  }
  
  if(!movementSensed)
  {
    digitalWrite(ONBOARD_LED, LOW);
    flag = false;
  }  
  delay(100);
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback_mqtt(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
}

void setup_mqtt() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback_mqtt);

}

void connect_mqtt() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(SENSORNAME, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      //sendState();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
