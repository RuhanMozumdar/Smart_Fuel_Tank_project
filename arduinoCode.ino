#include <SoftwareSerial.h>
#include <Servo.h>
//
Servo myservo;
const int triggerPin = 11;  //s
const int echoPin = 10;    //r
float pulse_width, distance;
SoftwareSerial GSM(7, 8);  //tx , rx
void setup() {

  Serial.begin(9600);
  GSM.begin(9600);
  myservo.attach(9); // Attach the Servo Motor to pin 9
  // Send AT command to check if the module is working
  GSM.println("AT");
  delay(1000);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Send AT command to set the module in SMS text mode
  GSM.println("AT+CMGF=1");
  delay(1000);

  // Send AT command to set the module to automatically receive SMS
  GSM.println("AT+CNMI=2,2,0,0,0");
  pinMode(2, OUTPUT);
  delay(1000);
  Serial.println("setup");
}

void loop() {
  if (GSM.available()) {
    delay(500);
    String message = "";
    while (GSM.available()) {
      message += (char)GSM.read();
    }
    Serial.println("SMS Received: " + message);
    if (message.indexOf("Run") > 0) {
      myservo.write(180); // rotate the servo motor to 180 degree
      Serial.println("open");
      digitalWrite(2, HIGH);
    }
    if (message.indexOf("Stop") > 0) {
      myservo.write(0); // rotate the servo motor to 0 degree
      Serial.println("close");
      digitalWrite(2, LOW);
    }
    if (message.indexOf("Count") > 0) {
      // put your main code here, to run repeatedly:
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);
      pulse_width = pulseIn(echoPin, HIGH);
      distance = ((pulse_width) * .0343) / 2;
      Serial.print("Distance = ");
      Serial.print(distance);
      Serial.println(" cm");
      delay(500);

    }

  }
}
