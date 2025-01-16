#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char *ssid = "Bathroom Cam 3";
const char *password = "12345678";

// GPIO pins for motor driver
#define ENA 16
#define ENB 17
#define IN1 5
#define IN2 18
#define IN3 19
#define IN4 21
#define leftSensor 34
#define rightSensor 35

// Web server on port 80
WebServer server(80);

// Function prototypes
void goForward();
void goBackward();
void turnLeft();
void turnRight();
void stopCar();
void sendCommandToCar(char command);

// Initializing the motor control mode
String mode = "remote"; // Default mode is line-following

void setup() {
  Serial.begin(115200);

  // Initialize motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  analogWrite(ENA, 255); // Max speed for Motor A
  analogWrite(ENB, 240); // Max speed for Motor B

  // Start Wi-Fi in access point mode
  WiFi.softAP(ssid, password);
  Serial.println("ESP32 Hotspot started!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Define web server routes
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <title>ESP32 Car Control</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
        <style>
          body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin: 20px;
            background-color: #f4f4f9;
          }
          h1 {
            font-size: 28px;
            margin-bottom: 20px;
          }
          .joystick-container {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
          }
          .direction-buttons {
            display: flex;
            justify-content: space-between;
            width: 250px;
            margin: 20px 0;
          }
          .vertical-buttons {
            display: flex;
            flex-direction: column;
            align-items: center;
          }
          button {
            width: 120px;
            height: 60px;
            font-size: 20px;
            margin: 10px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 10px;
            cursor: pointer;
            user-select: none;
            touch-action: none;
          }
          button:hover {
            background-color: #45a049;
          }
          @media (max-width: 600px) {
            .direction-buttons {
              width: 100%;
            }
            button {
              width: 80%;
              height: 60px;
              font-size: 22px;
            }
          }
        </style>
      </head>
      <body>
        <h1>ESP32 Car Control</h1>
        <div class="joystick-container">
          <div class="vertical-buttons">
            <button 
              id="forward" 
              onmousedown="startMoving('F')" 
              onmouseup="stopMoving()" 
              ontouchstart="startMoving('F')" 
              ontouchend="stopMoving()">Forward</button>
          </div>
          <div class="direction-buttons">
            <button 
              id="left" 
              onmousedown="startMoving('L')" 
              onmouseup="stopMoving()" 
              ontouchstart="startMoving('L')" 
              ontouchend="stopMoving()">Left</button>
            <button onclick="sendCommand('S')">Stop</button>
            <button 
              id="right" 
              onmousedown="startMoving('R')" 
              onmouseup="stopMoving()" 
              ontouchstart="startMoving('R')" 
              ontouchend="stopMoving()">Right</button>
          </div>
          <div class="vertical-buttons">
            <button 
              id="backward" 
              onmousedown="startMoving('B')" 
              onmouseup="stopMoving()" 
              ontouchstart="startMoving('B')" 
              ontouchend="stopMoving()">Backward</button>
          </div>
        </div>
        <br>
        <button onclick="toggleMode()">Toggle Mode</button>
        <p id="modeDisplay">Current Mode: Line</p>
        <script>
          function sendCommand(cmd) {
            fetch('/control?cmd=' + cmd)
              .then(response => response.text())
              .then(data => console.log(data));
          }
          function toggleMode() {
            fetch('/toggleMode')
              .then(response => response.text())
              .then(mode => {
                document.getElementById('modeDisplay').innerText = "Current Mode: " + mode;
              });
          }
          function startMoving(direction) {
            sendCommand(direction);
          }
          function stopMoving() {
            sendCommand('S');
          }
        </script>
      </body>
      </html>
    )rawliteral");
  });

  server.on("/control", HTTP_GET, []() {
    String cmd = server.arg("cmd");
    if (cmd.length() == 1) {
      sendCommandToCar(cmd[0]);
      server.send(200, "text/plain", "Command sent: " + cmd);
    } else {
      server.send(400, "text/plain", "Invalid command");
    }
  });

  server.on("/toggleMode", HTTP_GET, []() {
    if (mode == "line") {
      mode = "remote";
    } else {
      mode = "line";
    }
    server.send(200, "text/plain", mode);
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
  if (mode == "line") {
    int leftState = digitalRead(leftSensor);
    int rightState = digitalRead(rightSensor);

    if (leftState == HIGH && rightState == HIGH) {
      goForward();
    } else if (leftState == LOW && rightState == HIGH) {
      turnLeft();
    } else if (leftState == HIGH && rightState == LOW) {
      turnRight();
    } else {
      stopCar();
    }
  }
}

void sendCommandToCar(char command) {
  switch (command) {
    case 'F': goForward(); break;
    case 'B': goBackward(); break;
    case 'L': turnLeft(); break;
    case 'R': turnRight(); break;
    case 'S': stopCar(); break;
    default: Serial.println("Invalid command received"); break;
  }
}

void goForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void goBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
