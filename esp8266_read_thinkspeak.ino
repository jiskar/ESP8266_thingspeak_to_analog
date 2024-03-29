/*
 *  This sketch receives data from a HTTP GET requests to thingspeak service and sets an analog output to it's value
 *
 *  You need to get channel and privateKey at data.sparkfun.com and paste them
 *
 */

#include <EEPROM.h>
#include <ESP8266WiFi.h>

const char* ssid     = "YOURSSID";
const char* password = "password";

const char* host = "api.thingspeak.com";
const char* channel   = "...";
const char* privateKey = "...";  //the API read-key
String fieldName = "field1";  // the field to read from the specified channel

int outputValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT); // status LED
  pinMode(2, OUTPUT); // analog output
 
  analogWrite(2, outputValue);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Starting up. Setting output to ");
  Serial.println(outputValue);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  ESP.wdtFeed();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    ESP.wdtFeed();
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

int value = 0;

void loop() {
  delay(2000);
  ++value;

  Serial.print("connecting to ");
  Serial.print(host);
  
  digitalWrite(4, HIGH); //set LED on
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println(" .. connection failed");
    return;
  }
  else{
    Serial.println(" .. success");
  }
  ESP.wdtFeed();
  
  // We now create a URI for the request
  String url = "/channels/";
  url += channel;
  url += "/feeds/last/";
  url += "?key=";
  url += privateKey;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  ESP.wdtFeed();

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);

  // Read the reply from server and strip out the JSON part
  String JSON = "";
  bool beginChar = false;
  while(client.available() || !beginChar ){
    ESP.wdtFeed();
    char in = client.read();
    if (in == '{') {
        beginChar = true;
    }
    if (beginChar) JSON += (in);
    if (in == '}') {
        break;
    }
    delay(1);
  }
  
  digitalWrite(4, LOW); //set LED off
  Serial.print("response: ");
  Serial.println(JSON);
  
  //extract the desired value from the JSON string
  int start_index = JSON.indexOf(fieldName + "\":") + fieldName.length() + 3;
  int end_index = JSON.indexOf("\"}");
  String stringValue = JSON.substring(start_index, end_index); 
  int newValue = stringValue.toInt();
  if (newValue != outputValue){
    outputValue = newValue;
    Serial.print("output has CHANGED to ");
    analogWrite(2, outputValue);
  }
  else{
    Serial.print("output unchanged, value: ");
  }
  Serial.println(outputValue);
  Serial.println("closing connection");
  Serial.println();
}



