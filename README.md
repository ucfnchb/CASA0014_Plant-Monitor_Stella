# Plant-Monitor_Stella
A repo for the plant monitor project, developed at UCL, CASA.


1st phase Setting up ESP8266 WiFi: 
1.	Arduino IDE 2.2.1:  a code editor with powerful functions which is easy to use.
2.	MQTT Explorer:  to inspect data travelling through a MQTT server. The monitor would gather plant data and send it to CASA’s Living Lab MQTT broker. MQTT is a messaging protocol for capturing IoT device data in a server (the broker).
3.	Huzzah ESP8266: https://www.adafruit.com/product/2821
4. Install the SiLabs CP2104
5. Install the ESP8266 Board Package in the Arduino
6. Setup ESP8266
*Adafruit Feather HUZZAH ESP8266
*80 MHz as the CPU frequency
*Upload Speed 115200 baud
*Matching COM port
7. Blink Test, LED blinking indicates that connection successful. [Workshop Learning process/1_blink test]
8. Connecting via WiFi, connecting to a webserver, include <ESP8266WiFi> library.[Workshop Learning process/2_wifi connection]
9 Buit-in clock as most Arduino doesn’t have a built-in clock so each time it starts, we need to tell it the time. <ezTime.h> for time capture on Arduino. [Workshop Learning process/3_built-in Time]
 
2nd phase: getting familiar with MQTT and data flowing across the web:

1. MQTT: is an OASIS standard messaging protocol for the Internet of Things (IoT). It is designed as an extremely lightweight publish/subscribe messaging transport that is ideal for connecting remote devices with a small code footprint and minimal network bandwidth.
2. <PubSubClient.h> for MQTT access [Workshop Learning process_4_MQTT]
3. Using MQTT Explorer to watch a TOPIC
  *Send a control message via MQTT create a new topic to publish
  *student/CASA0014/plant/ucfnchb/inTopic [Workshop Learning process/5_MQTT_sendTopic]

4. Sensing soil + environment: 
  *DHT22 temperature/humidity sensor + pair of nails 
Principle: The basic principle is to measure the resistance of the soil between two nails a distance apart. The more moisture there is in the soil the lower the resistance.
  *Observation: challenges whilst using the nails from electrolysis to working out what the analog reading means, it's dependent on length of nails, depth in soil, type of soil, distance between nails.
  *Fritzing diagram
5. Include the library: <DHT_U.h> , for DHT22 sensor.
6. Sending Soil Data to MQTT, [Workshop Learning process/6_Soil Data to MQTT]

Phase 2 Conculsion:
libraries: 
<ESP8266WiFi.h> for WiFi
<ezTime.h> for time capture on Arduino
<PubSubClient.h> for MQTT access
<DHT.h> for the DHT22 sensor
[Workshop Learning process/6_Soil Data to MQTT] and [Workshop Learning process/5_MQTT_sendTopic] include "arduino_secrets.h" script to hide private information


Photosensitive_resistor: 
The code executed on the Arduino Uno is referred to as the photosensitive resistor. The objective of this script is to receive sequential signals, detect the light intensity, and ring the buzzer. The condition distance was established as 100 cm, indicating that the buzzer will be activated to alert the intruder when the object comes within this range.
