# Project 01 — ESP32 Event-Driven GPIO Controller

## Overview

This project demonstrates the progressive development of an event-driven GPIO controller using an ESP32.

The implementation was developed through five levels, starting from basic GPIO output and progressing to interrupt-based input handling, software debouncing, finite state machine control, non-blocking timing, hardware PWM, diagnostics, and initialization/error handling.

The project was implemented and validated directly on ESP32 hardware.

---

## Project Objective

The objective of this project is to build a structured embedded application while progressively applying fundamental embedded software concepts:

- GPIO configuration
- Digital input/output
- Interrupt handling
- ISR implementation
- Software debouncing
- Event-driven programming
- Finite State Machine (FSM)
- Non-blocking timing
- Hardware PWM
- Runtime diagnostics
- Initialization and error handling
- Hardware/software validation

---

# Hardware

- ESP32 development board
- External LED
- 330 Ω current-limiting resistor
- Breadboard
- Jumper wires
- USB cable

---

# Software

- Arduino IDE
- ESP32 board support package by Espressif Systems
- Embedded C/C++

No external library is required for the GPIO, interrupt, PWM, and timing implementation used in this project.

---

# Hardware Connections

## LED

The external LED is connected to GPIO 2 through a 330 Ω current-limiting resistor.

```text
ESP32 GPIO 2
     |
    330 Ω
     |
    LED
     |
    GND

    Button / Input

GPIO 17 is configured using the ESP32 internal pull-up.

A jumper wire is used as a temporary switch between GPIO 17 and GND.

ESP32 GPIO 17
     |
 Jumper Wire
     |
    GND

Input behavior:

Jumper disconnected → HIGH
Jumper connected     → LOW

A falling-edge interrupt detects the transition.

Development Progression
Level 1 — Basic GPIO
Objective

Configure an ESP32 GPIO as an output and control an external LED.

Implementation
GPIO 2 configured as output
External LED connected through a 330 Ω resistor
LED controlled using digital output
Basic timing implemented using delay()
Concepts Demonstrated
GPIO output configuration
Digital output
LED current limiting
Basic hardware validation
Source

gpio_blink.ino

Level 2 — Interrupt and Debouncing
Objective

Introduce GPIO input handling using an interrupt and software debouncing.

Implementation
GPIO 17 configured as INPUT_PULLUP
Falling-edge GPIO interrupt configured
Interrupt Service Routine detects the input event
volatile event flag used between ISR and application code
Software debounce timing implemented
LED state toggled after a valid input event
Concepts Demonstrated
GPIO input
Internal pull-up
GPIO interrupt
ISR
volatile
Software debouncing
Event-driven input handling
Source

project1_L2.ino

Level 3 — Finite State Machine and Non-Blocking Timing
Objective

Replace simple LED toggling with a finite state machine and non-blocking timing.

Operating States
OFF
 ↓
ON
 ↓
SLOW BLINK
 ↓
FAST BLINK
 ↓
OFF
State Behavior
State	LED Behavior
OFF	LED continuously OFF
ON	LED continuously ON
SLOW BLINK	Approximately 1 second interval
FAST BLINK	Approximately 250 ms interval
Implementation
Finite State Machine using enumerated states
Button events trigger state transitions
millis() used for non-blocking timing
delay() removed from LED state timing
Serial output used for state diagnostics
Concepts Demonstrated
Finite State Machine
State transitions
Non-blocking timing
Event-driven application logic
Serial diagnostics
Source

project1_L3.ino

Level 4 — Hardware PWM
Objective

Introduce hardware PWM for LED brightness control while retaining the state-machine architecture.

PWM Configuration
Frequency   : 5000 Hz
Resolution  : 8-bit
Full        : 255
Dim         : 75
Implementation
ESP32 LEDC hardware PWM
PWM-based LED brightness control
Full and dim brightness levels
Non-blocking timing retained
FSM architecture retained
Concepts Demonstrated
Hardware PWM
PWM duty cycle
LED brightness control
ESP32 LEDC peripheral
Non-blocking control
FSM integration
Source

Project1_L4.ino

Level 5 — Structured Embedded Implementation
Objective

Refactor the controller into a cleaner embedded software structure.

Architecture
                 GPIO Input
                     |
                     v
              GPIO Interrupt
                     |
                     v
               Button Event
                     |
                     v
             Application Layer
                     |
                     v
             Finite State Machine
                     |
                     v
              LED Controller
                     |
                     v
              GPIO / PWM Hardware
Implementation
Hardware configuration separated from application configuration
enum class used for LED operating modes
Dedicated LED control functions
Centralized state-transition handling
Interrupt-based event generation
Non-blocking timing using millis()
Periodic runtime diagnostics
PWM initialization status checked
Safe handling of initialization failure
Serial logging for system status and state transitions
Concepts Demonstrated
Embedded software structure
Hardware abstraction
Event-driven architecture
Finite State Machine
Interrupt handling
Non-blocking execution
Hardware PWM
Diagnostics
Basic error handling
Source

project1_L5.ino

Final System Flow
             GPIO 17 Input
                    |
                    v
             GPIO Interrupt
                    |
                    v
              Debouncing
                    |
                    v
             Button Event
                    |
                    v
          Finite State Machine
                    |
        +-----------+-----------+
        |           |           |
        v           v           v
       OFF         ON        BLINK
                                |
                                v
                         Non-Blocking Timer
                                |
                                v
                              PWM
                                |
                                v
                             GPIO 2
                                |
                                v
                              LED
Testing and Validation

The implementation was tested directly on the ESP32 hardware at each development level.

Level 1
 LED turns ON
 LED turns OFF
 GPIO output validated
Level 2
 GPIO input detected
 Interrupt triggered
 LED toggled
 Debouncing validated
Level 3
 OFF → ON
 ON → SLOW BLINK
 SLOW BLINK → FAST BLINK
 FAST BLINK → OFF
 Non-blocking timing validated
Level 4
 PWM initialized
 Full brightness validated
 Dim brightness validated
 Slow blink validated
 Fast blink validated
 Non-blocking behavior validated
Level 5
 Hardware initialization successful
 State transitions validated
 Runtime diagnostics validated
 Debouncing validated
 Non-blocking operation validated
 Long-running operation tested
Runtime Diagnostics

Example startup output:

[INIT] Initializing hardware...
[INIT] Hardware initialization OK
[SYSTEM] Controller started
[SYSTEM] Current mode: OFF

Example state transition:

State transition: OFF -> ON

Example diagnostic message:

[DIAGNOSTIC] Mode=SLOW_BLINK | LED=ON
Key Embedded Concepts
GPIO
 ├── Digital Input
 └── Digital Output

Interrupts
 ├── GPIO Interrupt
 ├── ISR
 └── volatile Event Flag

Timing
 └── Non-Blocking millis()

Control
 └── Finite State Machine

Output
 └── Hardware PWM

Reliability
 ├── Debouncing
 ├── Initialization Checks
 └── Runtime Diagnostics
Project Outcome

The project progressed from a basic GPIO LED demonstration into a structured event-driven embedded controller.

The final implementation combines GPIO input handling, interrupts, debouncing, finite state-machine control, non-blocking timing, hardware PWM, diagnostics, and basic initialization error handling.

This project establishes the GPIO and embedded software foundation for the subsequent communication projects.
