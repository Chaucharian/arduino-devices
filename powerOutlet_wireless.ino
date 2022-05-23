/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  
  Code compatible with ESP8266 Boards Version 3.0.0 or above 
  (see in Tools > Boards > Boards Manager > ESP8266)
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

// ULTRASONIC SENSOR
#define echoPin 14 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 12 //attach pin D3 Arduino to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

// WIFI
const char* ssid = "MARCOS"; // "Fibertel WiFi448 2.4GHz";
const char* password = "dorc2785";
String serverName = "http://notgamp.marplacode.com/api/v1/enviroment/power";
float payloardResponse[3];

// RELAYS
int relayPin1 = 16;
int relayPin2 = 05;
int relayPin3 = 04;
int relayPin4 = 0;

// client id
String clientId = "36e7933d-a16b-458c-9208-99e59f89ee8d"; // OUTLET 
WiFiClient client;
HTTPClient http;

void setup() {
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin3, HIGH);
  digitalWrite(relayPin4, HIGH);

  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
     makeRequest();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}


void makeRequest() {
  String url = serverName;
  http.begin(client, url.c_str());
  String bearer = "Bearer "+clientId;
  http.addHeader("Authorization", bearer);
  int httpResponseCode = http.GET();
 
  if(httpResponseCode>0)
  { 
      String payload = http.getString();    //Get the response payload from server

      String response = "{}";
      response = http.getString();
      JSONVar parsedResponse = JSON.parse(response);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(parsedResponse) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("response");
      Serial.println(parsedResponse);
    
      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = parsedResponse.keys();
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = parsedResponse[keys[i]];
        payloardResponse[i] = double(value);
      }

    setPowerStatus();

  }
  else
  {
    Serial.println("Error in response");
    Serial.println(httpResponseCode);
  }
      // Free resources
      http.end();
  }

void setPowerStatus() {
    if(payloardResponse[0]) { // RELAY 1
      digitalWrite(relayPin1, LOW);
      } else {
    digitalWrite(relayPin1,HIGH);  
    }
    
    if(payloardResponse[1]) { // 2
      digitalWrite(relayPin2, LOW);
      } else {
    digitalWrite(relayPin2,HIGH);  
    }
    
    if(payloardResponse[2]) { // 3
     digitalWrite(relayPin3, LOW);
      } else {
    digitalWrite(relayPin3,HIGH);  
    }
    
    if(payloardResponse[3]) { // 
      digitalWrite(relayPin4,LOW);
     } else {
      digitalWrite(relayPin4, HIGH);  
    }
    
  }
  
