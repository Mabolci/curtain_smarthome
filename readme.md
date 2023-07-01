# Smarthome - Curtain Controller
ESP32 micorcontroller code for Domoticz control of window curtain.

This is part of submission for university subject with the shortest acronime possible -- UMPANUMiW

## Hardware 
* ESP32 (ESP8266 can also be used, tho the part of code responsible for stepper steps has to be changed to pulsating as ESP8266 crashes if core is busy for too long)
* ULN2004A pr similar 4bit stepper motor controller (less/more acurate controllers can be used)
* Stepper motor
* 2.2V - 3.6V power supply for ESP32
* 5V - 12V power supply for stepper motor (the more the better, but also - hotter)

## Setup and Usage
### Software
Create a secret.cpp file in src/ folder with this structure:
```cpp
char* ssid = "YOUR_WIFI_NETWORK_NAME";
char* password = "YOUR_WIFI_PASSWORD";
char* mqtt_username = "umpanumiw";
char* mqtt_pwd = "umpanumiw";
```

### Hardware
Connect your ESP32 to stepper controller in the following way:
| ESP32 GPIO    | Stepper Controller |
| ------ | ------- |
| 13    | IN1 |
| 12    | IN2 |
| 14    | IN3 |
| 27    | IN4 |

### Debugging
Running code logs can be seen at runtime using a serial motnitor. Logs are sent using 115200 boundrate. To translate exceptions core dumps use exception decoder.

# Translator
ESP family microcontrollers can only recieve MQTT messages of maximum length of about 244 characters. As by default Domoticz messages are much longer, a proxy is needed. This proxy shortens the messages written to *domoticz/out* MQTT broker topic by removing unnecessary JSON fields, and forwards them to *esp/out* topic.

## Usage
Run translator.py file using python 3.9 or higher. Additional libraries are required, all avilable via PIP.

## Configuration
The translator can be used for other MQTT devices, not only the curtain. In order to change forwarded messages edit the *translator.conf* file. Each line represent a single JSON field that will be forwarded.

Ex.

```
idx
svalue1
nvalue
```

Above config file will forward the **idx**, **svalue1** and **nvalue** fields.

Recieved and sent messages are visible in console.