// SMARS Bluetooth and IR Remote Control
// Kevin McAleer
// April 2019
// May 2019 - added buzzer feature
// March 2026 - added IR remote support
// Requires a Fundomoto shield

#include <IRremote.h>

// Motor control pins
int ch_A_Direction = 12;
int ch_B_Direction = 13;
int ch_A_speed = 10;
int ch_B_speed = 11;

// IR setup
int IR_PIN = 2;
IRrecv irrecv(IR_PIN);
decode_results results;

char state = 0;
int delaylength = 1000;
int buzzerPin = 4;

// IR Remote button codes - UPDATE THESE WITH YOUR REMOTE'S CODES
// To find your codes, use Serial Monitor and press buttons
#define IR_FORWARD  0xFF629D
#define IR_BACKWARD 0xFFA857
#define IR_LEFT     0xFF22DD
#define IR_RIGHT    0xFFC23D
#define IR_STOP     0xFF02FD
#define IR_BUZZER   0xFF42BD

void setup() {
  Serial.begin(9600, SERIAL_8N1);
  Serial.println("SMARSFan OS 1.2");
  Serial.println("---------------");
  Serial.println("Controls: Serial + IR Remote");
  
  // Initialize IR receiver
  irrecv.enableIRIn();
  Serial.println("IR Receiver Ready on Pin 2");

  // establish motor direction toggle pins
  pinMode(ch_A_Direction, OUTPUT);
  pinMode(ch_B_Direction, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void buzz(){
  digitalWrite(buzzerPin, HIGH);
  delay(delaylength / 2);
  digitalWrite(buzzerPin, LOW);
}

void forward() {
  Serial.println("MOTORS: FORWARD");
  digitalWrite(ch_A_Direction, LOW);
  digitalWrite(ch_B_Direction, HIGH);
  analogWrite(ch_A_speed, 255);
  analogWrite(ch_B_speed, 255);
  delay(delaylength);
  analogWrite(ch_A_speed, 0);
  analogWrite(ch_B_speed, 0);
}

void backward() {
  Serial.println("MOTORS: BACKWARD");
  digitalWrite(ch_A_Direction, HIGH);
  digitalWrite(ch_B_Direction, LOW);
  analogWrite(ch_A_speed, 255);
  analogWrite(ch_B_speed, 255);
  delay(delaylength);
  analogWrite(ch_A_speed, 0);
  analogWrite(ch_B_speed, 0);
}

void left() {
  Serial.println("MOTORS: LEFT");
  digitalWrite(ch_A_Direction, HIGH);
  digitalWrite(ch_B_Direction, HIGH);
  analogWrite(ch_A_speed, 255);
  analogWrite(ch_B_speed, 255);
  delay(delaylength);
  analogWrite(ch_A_speed, 0);
  analogWrite(ch_B_speed, 0);
}

void right() {
  Serial.println("MOTORS: RIGHT");
  digitalWrite(ch_A_Direction, LOW);
  digitalWrite(ch_B_Direction, LOW);
  analogWrite(ch_A_speed, 255);
  analogWrite(ch_B_speed, 255);
  delay(delaylength);
  analogWrite(ch_A_speed, 0);
  analogWrite(ch_B_speed, 0);
}

void fullstop() {
  Serial.println("MOTORS: STOP");
  digitalWrite(ch_A_Direction, HIGH);
  digitalWrite(ch_B_Direction, HIGH);
  analogWrite(ch_A_speed, 0);
  analogWrite(ch_B_speed, 0);
  delay(delaylength);
}

void loop() {
  // Check for IR remote commands
  if (irrecv.decode(&results)) {
    // Print received code for debugging
    Serial.print("IR Code: 0x");
    Serial.println(results.value, HEX);
    
    // Process IR commands
    switch(results.value) {
      case IR_FORWARD:
        forward();
        break;
      case IR_BACKWARD:
        backward();
        break;
      case IR_LEFT:
        left();
        break;
      case IR_RIGHT:
        right();
        break;
      case IR_STOP:
        fullstop();
        break;
      case IR_BUZZER:
        buzz();
        break;
      default:
        // Unknown code - just print it
        Serial.println("Unknown IR code - update defines if needed");
        break;
    }
    
    irrecv.resume(); // Ready for next IR signal
  }
  
  // Check for serial commands (Bluetooth or USB)
  if (Serial.available() > 0) {
    state = Serial.read();
    Serial.println(state);

    if (state == 'u') {
      forward();
      state = 0;
    }
    else if (state == 'd') {
      backward();
      state = 0;
    }
    else if (state == 'l') {
      left();
      state = 0;
    }
    else if (state == 'r') {
      right();
      state = 0;
    }
    else if (state == 's') {  // FIXED: Changed from "s" to 's'
      fullstop();
      state = 0;
    }
    else if (state == 'b') {
      buzz();
      state = 0;
    }
  }
}
