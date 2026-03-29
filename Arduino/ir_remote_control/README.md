# IR Remote Control & Auto-Roaming Setup Guide

## Hardware Setup

### Components Required:
1. **IR Receiver** (on Pin 2)
2. **HC-SR04 Ultrasonic Sensor** (on Pins 7 & 8)
3. **L298P Motor Board** (already connected)
4. **9V Battery** (already connected)

### IR Receiver Pins:
- `-` (GND) → Arduino GND
- Middle pin → Arduino 5V
- `S` (Signal) → Arduino Pin 2

### HC-SR04 Ultrasonic Sensor Pins:
- **VCC** → Arduino 5V
- **TRIG** → Arduino Pin 7
- **ECHO** → Arduino Pin 8
- **GND** → Arduino GND

## Software Installation

### Step 1: Install IR Library
1. Open Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for "IRremote"
4. Install "IRremote" by shirriff/z3t0

### Step 2: Find Your Remote's Codes
1. Open `ir_remote_control.ino` in Arduino IDE
2. Upload to your Arduino
3. Open Serial Monitor (Tools → Serial Monitor)
4. Set baud rate to **9600**
5. Point your IR remote at the receiver
6. Press each button and note the codes displayed (e.g., `0xFF18E7`)

### Step 3: Update Button Codes
Edit these lines in the code with your remote's actual codes:
```cpp
#define IR_FORWARD  0xFF18E7  // Replace with your UP button code
#define IR_BACKWARD 0xFF4AB5  // Replace with your DOWN button code
#define IR_LEFT     0xFF10EF  // Replace with your LEFT button code
#define IR_RIGHT    0xFF5AA5  // Replace with your RIGHT button code
#define IR_STOP     0xFF38C7  // Replace with your OK/SELECT button code
#define IR_BUZZER   0xFF7A85  // Replace with any extra button code
#define IR_AUTO     0xFFE01F  // Replace with AUTO-ROAMING toggle button code
```

### Step 4: Upload and Test
1. Save the modified code
2. Uploamanual control with your IR remote
4. Test auto-roaming mode!

## Features

### Manual Control
**IR Remote Control:** Control robot wirelessly
- UP = forward
- DOWN = backward  
- LEFT = left turn
- RIGHT = right turn
- OK/SELECT = stop
- Custom button = buzzer
- **AUTO button = Toggle auto-roaming mode**

**Serial Control:** Still works via USB or Bluetooth
- `u` = forward
- `d` = backward
- `l` = left
- `r` = right
- `s` = stop
- `b` = buzzer
- **`a` = Toggle auto-roaming mode**

### Auto-Roaming Mode (NEW!)
When activated, the robot will:
1. **Move forward** autonomously
2. **Detect obstacles** at 20cm distance
3. **Stop and back up** when obstacle detected
4. **Scan left and right** to measure distances in both directions
5. **Choose the path** with the most space
6. **Turn and continue** in the best direction
7. **Repeat** the process continuously

To activate auto-roaming:
- Press the AUTO button on your IR remote (double beeps = ON, single beep = OFF)
- Or send 'a' via serial monitor
- Press AUTO or STOP button again to exit auto-roaming mode

### Behavior Details
- **Safe Distance:** Robot stops when obstacle < 20cm
- **Scanning:** Turns left and right to measure available space
- **Smart Navigation:** Chooses direction with maximum clearance
- **Continuous Operation:** Runs until you manually stop it

## Improvements Made
- ✅ Added IR remote support on pin 2
- ✅ Added HC-SR04 ultrasonic sensor on pins 7 & 8
- ✅ Auto-roaming mode with intelligent obstacle avoidance
- ✅ Scans left/right to find best path
- ✅ Fixed bug: changed `"s"` to `'s'` for stop command
- ✅ Added debug output to Serial Monitor
- ✅ Supports IR, serial, AND autonomous control modes
- ✅ Supports both IR and serial control simultaneously
