
#include <ESP8266WiFi.h> // connect to webserver 
#include <ESP8266WebServer.h> // connect to WiFi
#include <ezTime.h> // time capture on Arduino
#include <PubSubClient.h> // MQTT access
#include <DHT.h> // DHT sensor
#include <DHT_U.h>//  connect to DHT sensor 
#include "arduino_secrets.h" // hide my private WiFi details
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Sensors - DHT22 and Nails
uint8_t DHTPin = 12;        // on Pin 2 
uint8_t soilPin = 0;      // ADC or A0 pin on Huzzah
float Temperature;
float Humidity;
int Moisture = 1; // initial value just in case web page is loaded before readMoisture called
int sensorVCC = 13;
int blueLED = 2;
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.


// hidden varidables for my Wifi and MQTT
const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;
const char* mqtt_server = "mqtt.cetools.org";

ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
Timezone GB; // UK Time and Date



void setup() {
  //LED light setup for a push command from MQTT for turning on and turning off
  // Turn the LED off by making the voltage HIGH
  pinMode(BUILTIN_LED, OUTPUT);     
  digitalWrite(BUILTIN_LED, HIGH);  

  // Set up the outputs to control the soil sensor
  // switch and the blue LED for status indicator
  pinMode(sensorVCC, OUTPUT); 
  digitalWrite(sensorVCC, LOW);
  pinMode(blueLED, OUTPUT); 
  digitalWrite(blueLED, HIGH);

  // open serial connection 
  Serial.begin(115200);
  delay(100);

  // start DHT sensor
  // begin to read the DHT
  pinMode(DHTPin, INPUT);
  dht.begin();

  // run initialisation functions for the wifi and webserver
  startWifi();
  startWebserver();
  syncDate();

  // start MQTT server
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);

}
// receive the web server requests
// and reading the sensor data

void loop() {
 
  server.handleClient();

delay(5000);
//changed to every 5 second repeating upload data
// receive the moisture levels and DHT data and Time
    readMoisture();
    sendMQTT();
    Serial.println(GB.dateTime("H:i:s")); 
  
  client.loop();
}

// check the resistance between my 2 nails
void readMoisture(){
  
  // power the sensor,2 nails
  digitalWrite(sensorVCC, HIGH);
  digitalWrite(blueLED, LOW);
  delay(100);
  // read the value from the sensor, grabbing the analog data
  Moisture = analogRead(soilPin);      

  // stop to power the sensor, 2 nails   
  digitalWrite(sensorVCC, LOW);  
  digitalWrite(blueLED, HIGH);
  delay(100);

  Serial.print("Wet ");
  Serial.println(Moisture);   // read the value from 2 nails
}

void startWifi() {
  // connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);// connect to the WiFi

  // to check to see if connected to the Wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void syncDate() {
  // get UK date and time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());

}

void sendMQTT() {
// To check if the connection is vaild
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
// to send the value of temperature 
  Temperature = dht.readTemperature(); 
  // Gets the values of the temperature
  snprintf (msg, 50, "%.1f", Temperature);
  Serial.print("Publish message for t: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnchb/temperature", msg);

//send the value of humidity
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  snprintf (msg, 50, "%.0f", Humidity);
  Serial.print("Publish message for h: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnchb/humidity", msg);

  //send the value of moisture
  //Moisture = analogRead(soilPin);   
  // moisture read by readMoisture function
  snprintf (msg, 50, "%.0i", Moisture);
  Serial.print("Publish message for m: ");
  Serial.println(msg);
  client.publish("student/CASA0014/plant/ucfnchb/moisture", msg);

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on 
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by setting the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect with clientID, username and password
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      //resubscribe
      client.subscribe("student/CASA0014/plant/ucfnchb/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void startWebserver() {
  // when connected and IP address obtained start HTTP server  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");  
}

// to performs the calculations for the web server page
void handle_OnConnect() {
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  server.send(200, "text/html", SendHTML(Temperature, Humidity, Moisture));
}

// a 404 error web page for the server
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

// creates the HTML for the web server
String SendHTML(float Temperaturestat, float Humiditystat, int Moisturestat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP8266 DHT22 Report</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP8266 Huzzah DHT22 Report</h1>\n";

  ptr += "<p>Temperature: ";
  ptr += (int)Temperaturestat;
  ptr += " C</p>";
  ptr += "<p>Humidity: ";
  ptr += (int)Humiditystat;
  ptr += "%</p>";
  ptr += "<p>Moisture: ";
  ptr += Moisturestat;
  ptr += "</p>";
  ptr += "<p>Sampled on: ";
  ptr += GB.dateTime("l,");
  ptr += "<br>";
  ptr += GB.dateTime("d-M-y H:i:s T");
  ptr += "</p>";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}