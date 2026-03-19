# Smart Classroom System (MATLAB + Arduino + IR + BLE)

## Overview
This project implements a smart classroom control system that allows users to control lighting and fan settings using a MATLAB-based graphical interface. The system uses a client-server architecture where MATLAB acts as the client and an Arduino R4 acts as the server, which then communicates wirelessly with an Arduino Mega using infrared (IR) signals.

The system supports both preset modes (Winter, Summer, Spring) and full manual configuration of lighting and fan levels.


## System Architecture

MATLAB UI (Client)
        │
        │ Serial Communication (UART)
        ▼
Arduino UNO R4 (Server - BLE + IR Transmitter)
        │
        │ NEC IR Protocol (Wireless)
        ▼
Arduino Mega (Receiver - PWM Control)
        │
        ▼
Lighting + Fan Actuation


## Features

- MATLAB GUI for real-time control
- Preset modes: Winter, Summer, Spring
- Manual control:
  - Light brightness (0–100%)
  - RGB color (0–255 each)
  - Fan speed (OFF / LOW / MED / HIGH)
- BLE-based user identification
- Wireless IR communication between controllers
- PWM-based actuator control


## Technologies Used

### Hardware
- Arduino UNO R4 WiFi (Main Controller)
- Arduino Mega 2560 (Receiver Controller)
- IR LED (Transmitter)
- IR Receiver Module
- RGB LED
- White LED
- DC Fan
- Breadboard, resistors, wiring

### Software
- MATLAB App Designer (GUI)
- MATLAB Serial Communication (`serialport`)
- Arduino IDE
- ArduinoBLE Library
- IRremote Library


## Communication Protocol

### MATLAB → Arduino R4 (Serial)

Preset Commands:
WINTER
SUMMER
SPRING


Manual Command:
MAN <light> <R> <G> <B> <fan>

Example:
MAN 120 255 180 255 200


### Arduino R4 → Arduino Mega (IR)

- Uses NEC protocol
- Sends 5 packets per command:
  - Light
  - Red
  - Green
  - Blue
  - Fan


## How to Run
1. Upload R4 transmitter code to Arduino UNO R4
2. Upload receiver code to Arduino Mega
3. Connect IR LED (R4) and IR receiver (Mega)
4. Run MATLAB App
5. Select preset or use manual configuration
6. System will update lighting and fan in real-time


## Future Improvements
- Add checksum/error detection for IR packets
- Replace IR with RF/WiFi for longer range
- Add feedback from receiver to MATLAB
- Implement smoother PWM transitions
- Expand BLE authentication system



## Author
Mohamad ElHaddad  
Computer Engineering Student


## License
This project is for educational purposes.
