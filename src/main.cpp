#include <Arduino.h>
#include <Stepper.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <secret.h>

// stepper definitions
#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27

const int stepsPerRevolution = 1024;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);
unsigned short int currentPos = 0;
unsigned short int stepMultiplier = 0;

// stepper definitions end

// wifi connection definitions
WiFiClient espClient;
PubSubClient client(espClient);
// wifi connection definitions end

// mqtt definitions
const char* mqtt_server = "1.tcp.eu.ngrok.io";
short int mqtt_port = 21589;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // deserialize json
  StaticJsonDocument<244> doc;
  deserializeJson(doc, payload, length);

  // read values
  short unsigned int idx = doc["idx"];
  if(idx == 4) {
    short unsigned int value = doc["svalue1"];
    short int diff = value - currentPos;

    currentPos = value;

    Serial.print("Stepping by: ");
    Serial.println(diff * stepMultiplier);

    myStepper.step(diff * stepMultiplier);
  } else if(idx == 5) {
    currentPos = 0;
    Serial.println("Resetting stepper position");
  } else if(idx == 17) {
    stepMultiplier = doc["svalue1"];
    Serial.print("Changed step multiplier to: ");
    Serial.println(stepMultiplier);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("JK_ESP32", mqtt_username, mqtt_pwd)) {
      Serial.print("connected with ");
      Serial.print(mqtt_username);
      Serial.println("!");
      // Subscribe
      Serial.println("Subscribing to topics:");
      Serial.println("esp/out/4");
      Serial.println("esp/out/5");
      Serial.println("esp/out/17");
      client.subscribe("esp/out/4");
      client.subscribe("esp/out/5");
      client.subscribe("esp/out/17");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// mqtt definitions end


void setup() {
  // initialize the serial port:
  Serial.begin(115200);

  // set the speed at 29 rpm:
  myStepper.setSpeed(29);

  // wifi connection starts
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // wifi connection ends

  // mqtt starts
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
}