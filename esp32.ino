#include <WiFi.h>

// Wi-Fi credentials
const char *ssid = "Car";
const char *password = "12345678"; // Change this to a desired password

// Serial communication pins (use GPIO pins available for UART)
#define TX_PIN 17
#define RX_PIN 16

// Create a hardware serial object
HardwareSerial mySerial(2);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  // Start the secondary hardware serial for communication with Arduino
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  // Start Wi-Fi in Access Point mode
  WiFi.softAP(ssid, password);

  // Print IP address for devices to connect
  Serial.println("ESP32 Hotspot started!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Send data to Arduino
  String message = "Hello from ESP32!";
  mySerial.println(message);

  // Debug message on Serial Monitor
  Serial.println("Message sent to Arduino: " + message);

  // Wait for 1 second before sending the next message
  delay(1000);
}