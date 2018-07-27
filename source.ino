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
                const size_t bufferSize = JSON_OBJECT_SIZE(9) + 230;
                DynamicJsonBuffer jsonBuffer(bufferSize);
                JsonObject& root = jsonBuffer.parseObject(payload); // parse string
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
                            
                String command1 = "ads.txt=\" " + String(ads_blocked_today) + "\""; // ads.txt="1234" string
                Serial.print(command1); // send string over serial
                endNextionCommand();    // send 0xff 3x
                delay(10);
                String command2 = "domains.txt=\" " + String(domains_being_blocked) + "\""; // domains.txt="1234" string 
                Serial.print(command2); // send string over serial
                endNextionCommand();    // send 0xff 3x
                delay(10);
                String command3 = "today.txt=\" " + String(ads_percentage_today) + "%\""; // today.txt="1234" string
                Serial.print(command3); // send string over serial
                endNextionCommand();  // send 0xff 3x
                delay(10);
                String command4 = "blocked.txt=\" " + String(dns_queries_today) + "\""; // blocked.txt="1234" string
                Serial.print(command4); // send string over serial
                endNextionCommand();    // send 0xff 3x
                http.end();             // close http connection
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
