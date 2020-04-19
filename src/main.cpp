
// Include Libraries
#include "Arduino.h"
#include "RFID.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Pin Definitions
#define RFID_PIN_RST	22
#define RFID_PIN_SDA	21

const char * ssid = "anuja1";
const char * password = "ar3k57u4";
const char * host = "192.168.43.82";
String device_id = "00000001";
String rfidNo, postData;
WiFiServer server(80);

// Global variables and defines

// object initialization
RFID rfid(RFID_PIN_SDA,RFID_PIN_RST);


// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    Serial.println("start");
    //initialize RFID module
    rfid.init();
    WiFi.mode(WIFI_OFF); //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);     //Connect to your WiFi router
    Serial.print("Connecting");
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
  //    digitalWrite(Led_OnBoard, LOW);
      delay(250);
      Serial.print(".");
  //    digitalWrite(Led_OnBoard, HIGH);
      delay(250);
    }
    Serial.println(WiFi.localIP());
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    // RFID Card Reader - RC522 - Test Code
    //Read RFID tag if present
    Serial.println("test");
    String rfidtag = rfid.readTag();
    //print the tag to serial monitor if one was discovered
    if(rfidtag != "None"){
      rfid.printTag(rfidtag);
      rfidNo = rfidtag;
      Serial.print("Tag: ");
      Serial.println(rfidNo);
      postData = "rfidNo=" + rfidNo + "&device_id=" + device_id;
      Serial.println(postData);
        HTTPClient http;
        http.begin("http://192.168.43.212:8084/samplepro/webresources/generic");              //Specify request destination
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
        int httpCode = http.POST(postData);   //Send the request
        String payload = http.getString();    //Get the response payload
        Serial.println(httpCode);   //Print HTTP return code
        Serial.println(payload);    //Print request response payload
        Serial.print("RFID No= ");
        Serial.print(rfidNo);
        const size_t capacity = JSON_OBJECT_SIZE(3 ) + JSON_ARRAY_SIZE(3) + 60;
        DynamicJsonDocument doc(capacity);

        // Parse JSON object
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
          return;
        }
        Serial.println(F("Response:"));
        Serial.println(doc["name"].as<char*>());
        delay(1000);
        http.end();  //Close connection
    }
    delay(1000);
}



