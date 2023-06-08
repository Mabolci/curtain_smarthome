#include <Arduino.h>
#include <Stepper.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define IN1 13
#define IN2 12
#define IN3 14
#define IN4 27

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

// for your motor


// initialize the stepper library on pins 8 through 11:

Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);


void setup() {

  // set the speed at 60 rpm:

  myStepper.setSpeed(60);

  // initialize the serial port:

  Serial.begin(115200);

}


void loop() {

  // step one revolution  in one direction:

  Serial.println("clockwise");

  myStepper.step(stepsPerRevolution);

  delay(500);


  // step one revolution in the other direction:

  Serial.println("counterclockwise");

  myStepper.step(-stepsPerRevolution);

  delay(500);

}