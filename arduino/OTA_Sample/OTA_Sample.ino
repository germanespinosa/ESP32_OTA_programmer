#include "ota.h"

// NETWORK CONFIG - adjust to your network
const char* wifi_ssid = "NETWORK";  
const char* wifi_password = "PASSWORD"; 
IPAddress wifi_local_IP(192, 168, 137, 165); 
IPAddress wifi_gateway(192, 168, 137, 1);    
IPAddress wifi_subnet(255, 255, 255, 0);  
IPAddress wifi_primaryDNS(8, 8, 8, 8);
IPAddress wifi_secondaryDNS(8, 8, 4, 4);

// DEFINES THE OTA UPDATER SERVER 
OTA(5000); // 5000 is the port where the updater will be listening

void setup() {
  
  // CONNECTS TO WIFI
  Serial.begin(115200);
  Serial.print("Connecting to wifi");
  if (!WiFi.config(wifi_local_IP, wifi_gateway, wifi_subnet, wifi_primaryDNS, wifi_secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  delay(100);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  
  // INITIALIZES THE OTA UPDATER SERVER ON THE WIFI NETWORK
  OTA_INIT();
  Serial.println("Connected");
}

void loop() {
  // CHECKS FOR UPDATES ON EACH LOOP
  OTA_CHECK_UPDATES();
  delay(10);
  // your stuff here
}
