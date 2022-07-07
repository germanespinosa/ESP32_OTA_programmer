#include "ota.h"

const char* wifi_ssid = "mazenet";
const char* wifi_password = "Albifrons2020";
IPAddress wifi_local_IP(192, 168, 137, 165);
IPAddress wifi_gateway(192, 168, 137, 1);
IPAddress wifi_subnet(255, 255, 255, 0);
IPAddress wifi_primaryDNS(8, 8, 8, 8);
IPAddress wifi_secondaryDNS(8, 8, 4, 4);


OTA(5000);

void setup() {
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
  OTA_INIT();
  Serial.println("Connected");
}

void loop() {
  OTA_CHECK_UPDATES();
  delay(10);
  // your stuff here
}
