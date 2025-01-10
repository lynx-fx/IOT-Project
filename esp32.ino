#include <WiFi.h>

const char* ssid = "Lynxx";
const char* password = "987654321";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Setting up Access Point...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point started with IP: ");
  Serial.println(IP);
}

void loop() {
  delay(1000);
}
