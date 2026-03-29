// SMARS Bluetooth and IR Remote Control with Autonomous Navigation
// Kevin McAleer
// April 2019
// May 2019 - added buzzer feature
// March 2026 - added IR remote support
// March 2026 - added HC-SR04 ultrasonic sensor and auto-roaming
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

// HC-SR04 Ultrasonic Sensor pins
int TRIG_PIN = 6;  // UPDATE THIS if you connected to a different pin
int ECHO_PIN = 7;  // UPDATE THIS if you connected to a different pin

// Auto-roaming mode
bool autoRoamMode = false;
int safeDistance = 20;  // Stop if obstacle within 20cm

// Distance logging
unsigned long lastDistanceLog = 0;
int distanceLogInterval = 500;  // Log distance every 500ms

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
#define IR_AUTO     0xFF52AD  // Auto-roaming toggle button

void setup() {
  Serial.begin(9600, SERIAL_8N1);
  Serial.println("SMARSFan OS 1.3");
  Serial.println("---------------");
  Serial.println("Controls: Serial + IR Remote + Auto-Roaming");
  
  // Initialize IR receiver
  irrecv.enableIRIn();
  Serial.println("IR Receiver Ready on Pin 2");

  // Initialize HC-SR04 sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("HC-SR04 Sensor Ready (Pins 6,7)");

  // establish motor direction toggle pins
  pinMode(ch_A_Direction, OUTPUT);
  pinMode(ch_B_Direction, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}


// Measure distance using HC-SR04
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms timeout
  long distance = duration * 0.034 / 2;  // Convert to cm
  
  return distance;
}

// Move forward continuously (no stop at end)
void moveForward() {
  digitalWrite(ch_A_Direction, LOW);
  digitalWrite(ch_B_Direction, HIGH);
  analogWrite(ch_A_speed, 255);
  analogWrite(ch_B_speed, 255);
}

// Move backward continuously
void moveBackward() {
  digitalWrite(ch_A_Direction, HIGH);
  digitalWrite(ch_B_Direction, LOW);
  analogWrite(ch_A_speed, 255);
  analogWrite(ch_B_speed, 255);
}

// Turn left continuously
void turnLeft() {
  digitalWrite(ch_A_Direction, HIGH);
  digitalWrite(ch_B_Direction, HIGH);
  analogWrite(ch_A_speed, 255);
  analogWrite(ch_B_speed, 255);
}

// Turn right continuously
void turnRight() {
  digitalWrite(ch_A_Direction, LOW);
  digitalWrite(ch_B_Direction, LOW);
  analogWrite(ch_A_speed, 255);
  analogWrite(ch_B_speed, 255);
}

// Stop motors
void stopMotors() {
  analogWrite(ch_A_speed, 0);
  analogWrite(ch_B_speed, 0);
}

// Scan left and right to find best direction
int findBestDirection() {
  Serial.println("Scanning for best path...");
  stopMotors();
  delay(200);
  
  // Measure distance straight ahead
  long centerDistance = getDistance();
  Serial.print("Center: ");
  Serial.print(centerDistance);
  Serial.println(" cm");
  delay(100);
  
  // Turn left and measure
  turnLeft();
  delay(500);  // Turn for 500ms
  stopMotors();
  delay(200);
  long leftDistance = getDistance();
  Serial.print("Left: ");
  Serial.print(leftDistance);
  Serial.println(" cm");
  delay(100);
  
  // Turn right (back to center then right)
  turnRight();
  delay(500);  // Back to center
  stopMotors();
  delay(200);
  
  turnRight();
  delay(500);  // Turn right
  stopMotors();
  delay(200);
  long rightDistance = getDistance();
  Serial.print("Right: ");
  Serial.print(rightDistance);
  Serial.println(" cm");
  delay(100);
  
  // Return to center position
  turnLeft();
  delay(500);
  stopMotors();
  delay(200);
  
  // Determine best direction (1=left, 2=right, 3=backward)
  if (leftDistance > rightDistance && leftDistance > centerDistance) {
    Serial.println("Best path: LEFT");
    return 1;
  } else if (rightDistance > leftDistance && rightDistance > centerDistance) {
    Serial.println("Best path: RIGHT");
    return 2;
  } else if (centerDistance > safeDistance) {
    Serial.println("Best path: FORWARD");
    return 0;
  } else {
    Serial.println("Best path: BACKWARD");
    return 3;
  }
}

// Auto-roaming behavior
void autoRoam() {
  long distance = getDistance();
  
  if (distance > 0 && distance < safeDistance) {
    // Obstacle detected!
    Serial.print("Obstacle detected at ");
    Serial.print(distance);
    Serial.println(" cm");
    
    stopMotors();
    delay(300);
    
    // Back up a bit
    moveBackward();
    delay(500);
    stopMotors();
    delay(200);
    
    // Find best direction
    int direction = findBestDirection();
    
    // Turn in best direction
    if (direction == 1) {  // Left
      turnLeft();
      delay(800);
    } else if (direction == 2) {  // Right
      turnRight();
      delay(800);
    } else if (direction == 3) {  // Backward - turn around
      turnRight();
      delay(1000);
    }
    
    stopMotors();
    delay(200);
  } else {
    // Path is clear, move forward
    moveForward();
  }
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
  // If auto-roaming mode is active, run autonomous navigation
  if (autoRoamMode) {
    autoRoam();
    
    // Still check for IR commands to allow mode exit
    if (irrecv.decode(&results)) {
      if (results.value == IR_AUTO || results.value == IR_STOP) {
        autoRoamMode = false;
        stopMotors();
        Serial.println("Auto-Roaming Mode: OFF");
        buzz();
      }
      irrecv.resume();
    }
    return;  // Skip manual controls while in auto mode
  }
  
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
      case IR_AUTO:
        // Toggle auto-roaming mode
        autoRoamMode = !autoRoamMode;
        if (autoRoamMode) {
          Serial.println("Auto-Roaming Mode: ON");
          buzz();
          delay(100);
          buzz();
        } else {
          Serial.println("Auto-Roaming Mode: OFF");
          stopMotors();
          buzz();
        }
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
    else if (state == 's') {
      fullstop();
      state = 0;
    }
    else if (state == 'b') {
      buzz();
      state = 0;
    }
    else if (state == 'a') {
      // Toggle auto-roaming via serial
      autoRoamMode = !autoRoamMode;
      if (autoRoamMode) {
        Serial.println("Auto-Roaming Mode: ON");
      } else {
        Serial.println("Auto-Roaming Mode: OFF");
        stopMotors();
      }
      state = 0;
    }
  }
  
  // Periodically log distance sensor reading for testing
  unsigned long currentMillis = millis();
  if (currentMillis - lastDistanceLog >= distanceLogInterval) {
    lastDistanceLog = currentMillis;
    long distance = getDistance();
    Serial.print("[SENSOR] Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}
