/**
 *  ESP32 & Nextion Display
 *  http://arejay.codes
 *  July 26, 2018
 *
 */


#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#define USE_SERIAL Serial

WiFiClient client; // wifi client object

const char* host         = "192.168.0.200"; // Pi-Hole IP
const char* ssid         = "SSID"; // Wi-Fi Name
const char* password     = "PASS"; // Wi-Fi Pass

void setup() {
    Serial.begin(9600);
    delay(1000);
    Start_WiFi(ssid,password);
}

void loop() {
       
   if((WiFi.status() == WL_CONNECTED)) {
        HTTPClient http;
        http.begin("http://"+ String(host) +"/admin/api.php?summary"); 
        int httpCode = http.GET();
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();       // save as string 'payload'
                const size_t bufferSize = JSON_OBJECT_SIZE(10) + 250;
                DynamicJsonBuffer jsonBuffer(bufferSize);
                JsonObject& root = jsonBuffer.parseObject(payload);
                JsonObject& response = root["response"];
                JsonObject& response_data0 = response["data"][0];
                const char* domains_being_blocked = root["domains_being_blocked"]; 
                const char* dns_queries_today = root["dns_queries_today"]; 
                const char* ads_blocked_today = root["ads_blocked_today"]; 
                const char* ads_percentage_today = root["ads_percentage_today"]; 
                const char* unique_domains = root["unique_domains"]; 
                const char* queries_forwarded = root["queries_forwarded"]; 
                const char* queries_cached = root["queries_cached"]; 
                const char* clients_ever_seen = root["clients_ever_seen"]; 
                const char* unique_clients = root["unique_clients"]; 
                const char* status_pihole = root["status"];
                            
                String command1 = "ads.txt=\" " + String(ads_blocked_today) + "\"";
                Serial.print(command1);
                endNextionCommand();
                delay(10);
                String command2 = "domains.txt=\" " + String(domains_being_blocked) + "\"";
                Serial.print(command2);
                endNextionCommand();
                delay(10);
                String command3 = "today.txt=\" " + String(ads_percentage_today) + "%\"";
                Serial.print(command3);
                endNextionCommand();
                delay(10);
                String command4 = "blocked.txt=\" " + String(dns_queries_today) + "\"";
                Serial.print(command4);
                endNextionCommand();
                delay(10);
                
                // check if Pi-Hole is enabled or disabled
                if (String(status_pihole) == "enabled") {  // status from the JSON requests equals 'enabled'
                   String command5 = "status.txt=\"ON\"";  // create string to send to Nextion over serial
                   Serial.print(command5);                 // send serial command
                   endNextionCommand();                    // send '0xff' 3x to let Nextion know its end of this serial command
                   delay(10);
                   Serial.print("status.pic=10");           // change backgeound of status.pic on the Nextion to image 10 stored on the displays memory
                   endNextionCommand();                     // send '0xff' 3x to let Nextion know its end of this serial command
                   delay(10);
                       
                }
                else {                                      // JSOnSrequest did not equal 'enabled'
                  String command6 = "status.txt=\"OFF\"";   // create string to send to Nextion over serial 
                  Serial.print(command6);                   // send serial command
                  endNextionCommand();                      // send '0xff' 3x to let Nextion know its end of this serial command
                  delay(10);  
                  Serial.print("status.pic=9");             // change backgeound of status.pic on the Nextion to image 9 stored on the displays memory
                  endNextionCommand();                      // send '0xff' 3x to let Nextion know its end of this serial command
                  delay(10);
                }
                
                http.end();                                 // end http socket
            }
        } else {
            // error - no wifi connection
       }
    }
        delay(10000); // 10 sec. refresh delay
}



void endNextionCommand(){   // 0xff must be sent 3x after
  Serial.write(0xff);       // each serial command
  Serial.write(0xff);
  Serial.write(0xff);
}


int Start_WiFi(const char* ssid, const char* password){   // connects to wifi
    int connAttempts = 0;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED ) {
        delay(500);
        if(connAttempts > 20) return -5;
        connAttempts++;
        }
    return 1;
}
