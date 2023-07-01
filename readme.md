# Smarthome - Curtain Controller
ESP32 micorcontroller code for Domoticz control of window curtain.
This is part of submission for university subject with the shortest acronime possible -- UMPANUMiW

# Hardware 
* ESP32 (ESP8266 can also be used, tho the part of code responsible for stepper steps has to be changed to pulsating as ESP8266 crashes if core is busy for too long)
* ULN2004A pr similar 4bit stepper motor controller (less/more acurate controllers can be used)
* Stepper motor
* 2.2V - 3.6V power supply for ESP32
* 5V - 12V power supply for stepper motor (the more the better, but also - hotter)

# Setup and Usage
## Software
Create a secret.cpp file in src/ folder with this structure:
```cpp
char* ssid = "YOUR_WIFI_NETWORK_NAME";
char* password = "YOUR_WIFI_PASSWORD";
char* mqtt_username = "umpanumiw";
char* mqtt_pwd = "umpanumiw";
```

## Hardware
| ESP32 GPIO    | Stepper Controller
| :---  |   | ---:
| 13    |   | IN1
| 12    |   | IN2
| 14    |   | IN3
| 27    |   | IN4