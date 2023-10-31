
## Plant Monitor:
A repo for the plant monitor project, developed at UCL CASA. This repository documents the process and resources used to develop a fully functioning plant monitor system. The purpose of this project is to create a plant monitor system that helps individuals keep track of the health and well-being of their plants. By monitoring temperature, humidity, moisture levels. In addition, the project features an alert system that uses a photosensitive resistor to detect changes in light intensity, thereby alerting the owner when their plant is surrounded by flies or pets, which may cause harm or damage. The system aims to provide timely alerts and ensure optimal conditions for plant growth. 


## Development: 
The Developing process consists of the four phases: 
- Phase 1: Setting up ESP8266 WiFi board
- Phase 2: MQTT connection 
- Phase 3: Setting up Raspberry Pi
 
Boards Used:
- An Adafruit Huzzah with ESP8266 WiFi board
- An Arduino Uno
- A Raspberry Pi

Components Used:
- A DHT temperature and humidity sensor
- A photoresistor LDR
- A buzzer 
- Two open wires for the moisture sensor
- Two nails for moisture sensing
- Resistors

# Phase 1: Setting up ESP8266 WiFi board
 ![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/5bb78ce4-722f-4768-84c0-422a21215c7d)
 
1.	ESP8266 WiFi board
2.	Install the SiLabs CP2104
3.	Install the ESP8266 Board Package in the Arduino
4.	Setup ESP8266
*Adafruit Feather HUZZAH ESP8266
*80 MHz as the CPU frequency
*Upload Speed 115200 baud
*Matching COM port
5.	[Workshop Learning process/1_blink test]: LED blinking indicates that connection successful. 
6.	[Workshop Learning process/2_wifi connection]: Get it connected to the internet.
7.	[Workshop Learning process/3_built-in Time]: Install a ezTime Library to grab the time from an NTP server on the internet, <ezTime.h> for time capture on Arduino. 


# Phase 2: MQTT connection 
![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/3a7cf2d2-2fbf-4f2d-ad1a-1f62bfbef93b)

1.	[Workshop Learning process_4_MQTT], Install and use the PubSubClient library.
2.	[Workshop Learning process/5_MQTT_sendTopic], Using MQTT Explorer to watch a TOPIC, and send a control message via MQTT create a new topic to publish.
 ![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/93df8c56-a4f4-4e56-a694-fff7bb44aee4)
![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/941190d2-f595-4c45-8b4a-0d782c327327)

 
# Phase 3: Setting up Raspberry Pi
1.	Setting up a RPi as a gateway as a datastore
https://www.tomshardware.com/reviews/raspberry-pi-headless-setup-how-to,6028.html, A installed RPi to capture and present sensor data. Control the system remotely using: VNC (Virtual Network Computing) and SSH (Secure Shell Protocol). 

2.	The Pi must be connected to the same local network as the Huzzah, and the three essential software components that need to be installed:

- InfluxDB (storing time series data) a database management system designed specifically for the storage of time series data. It provides efficient storage and retrieval mechanisms for this type of data, allowing for high-performance operations. In my project: create a bucket entitled mqtt-data, data should be transported into the 'mqtt-data' bucket.
 ![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/c44aed88-fc06-4e0f-b44f-9775b2837104)


- Telegraf (capturing MQTT data) a data collection agent that specializes in capturing data from MQTT (Message Queuing Telemetry Transport) sources. It is capable of efficiently gathering data from various MQTT devices and transmitting it to other systems for further processing.
 ![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/823e0c79-d2d3-475e-a133-4290a5d69629)


- Grafana (presenting the data on a dashboard) a visualization tool that enables the creation of interactive and customizable dashboards. It is particularly useful for presenting time series data in a visually appealing and informative manner. Grafana allows users to create and configure dashboards to display data from many sources, including InfluxDB and Telegraf. In my project: Grafana should then be configured to grab data from the mqtt-data bucket,  and create a data source that references my localhost IP.
 ![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/1a79b86a-79ce-43ea-a162-203270540c50)

![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/1b64a760-1a12-4367-b0ea-77f9ea529c98)

 

## Plant Monitor Setup and Deployment

Sensing soil + environment: 
DHT22 temperature/humidity sensor + pair of nails 
Principle: The basic principle is to measure the resistance of the soil between two nails a distance apart. The more moisture there is in the soil the lower the resistance.

Code: 
6_Soil_Data_to_MQTT.ino: manages the Huzzah, its WiFi, data capture, and serial communication with the Arduino Uno.
![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/8da05d6b-5ed1-431e-a976-a5198eea0fdc)

 
Sensing light intensity and trigger the buzzer 
A photoresistor + a buzzer
The objective of this script is to receive sequential signals, detect the light intensity, and ring the buzzer. The condition distance was established as 100 cm, indicating that the buzzer will be activated to alert the intruder when the object comes within this range.
Code: 
photosensitive_resistor.ino receives lighting signals and trigger the buzzer on the Arduino Uno.
 ![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/e7e115ed-8f56-4dba-85fb-f43abb5e10b2)
![image](https://github.com/ucfnchb/Plant-Monitor_Stella/assets/146333771/722d10fc-35ca-4e7b-a9c5-b1ee97102ed1)


 


