//MOTOR1 PINS
int ena = 5;
int in1 = 6;
int in2 = 7;
int in3 = 9;
int in4 = 8;
int enb = 10;
int leftSensor = 11;
int rightSensor= 12;
int powerRight = 13;
String mode = "line";

void setup() {
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(powerRight, OUTPUT);
  digitalWrite(powerRight, HIGH);
}

void loop() {
  if (mode == "line"){
    int leftState = digitalRead(leftSensor);
    int rightState = digitalRead(rightSensor);
    
    if (leftState == HIGH && rightState == HIGH){
      goForward();
    } else if( leftState == LOW && rightState == HIGH){
      goLeft();
    } else if (leftState == HIGH && rightState == LOW){
      goRight();
    } else{
      stop();
    }
  } else if (mode == "remote"){

  }
}

void goForward(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena, 255);
  
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enb, 255);
  delay(500);
}

void goBbackward(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena, 255);
  
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enb, 255);
  delay(500);
}

void goRight(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  analogWrite(ena, 255);
  
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enb, 255);
  delay(500);
}

void goLeft(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena, 255);
  
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  analogWrite(enb, 255);
  delay(500);
}

void stop(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  analogWrite(ena, 255);
  
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  analogWrite(enb, 255);
  delay(500);
}