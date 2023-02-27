#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

#define BEDROOM 23
#define LIVINGROOM 22
#define HALLWAY 21
//username: Roy

char * endpoint = "https://ecse-three-led-api.onrender.com";

void setup() 
{
  pinMode(BEDROOM, OUTPUT);
  pinMode(LIVINGROOM, OUTPUT);
  pinMode(HALLWAY, OUTPUT);

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
 {
  if(WiFi.status()== WL_CONNECTED)
  {
    
    HTTPClient http;  
    String http_response;


    http.begin(endpoint);
    http.addHeader("/api/state","X-API-Key",username);
    int httpResponseCode = http.GET();
   
    if (httpResponseCode > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else
      {   
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();

      StaticJsonDocument<1024> doc;

      doc["Bed Room"] = HIGH;
      doc["Living Room"] = HIGH;
      doc["Hall Way"] = HIGH;

      DeserializationError error = deserializeJson(doc, http_response);

      if (error) 
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      const char* Bed_Room = doc["Bed Room"]; // "High"
      const char* Living_Room = doc["Living Room"]; // "Low"
      const char* Hall_Way = doc["Hall Way"]; // High

        if(httpResponseCode >= 200 && httpResponseCode < 300)
        {
        digitalWrite(BEDROOM, !digitalRead(BEDROOM));
        digitalWrite(LIVINGROOM, !digitalRead(LIVINGROOM));
        digitalWrite(HALLWAY, !digitalRead(HALLWAY));
        }
  }
  else {
    return;
  }
}