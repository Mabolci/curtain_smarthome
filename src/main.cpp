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

// stepper definitions end

// wifi connection definitions
WiFiClient espClient;
PubSubClient client(espClient);
// wifi connection definitions end

// mqtt definitions
const char* mqtt_server = "1.tcp.eu.ngrok.io";
short int mqtt_port = 21589;

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("JK ESP32", mqtt_username, mqtt_pwd)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
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
  reconnect();
}


void loop() {
  // static char buffer[70];
  if (!client.connected()) {
    reconnect();
  }

  // myStepper.step(stepsPerRevolution);

  
  client.loop();
}