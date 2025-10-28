#  Xintong Shao CASA0014 Coursework

##  Project Overview
This project is developed for the CASA0014 coursework.  
It demonstrates an **IoT-based adaptive lighting system** using the **Arduino MKR WiFi 1010**, a **VEML7700 light sensor**, and a **touch sensor**.  
The system dynamically changes its lighting mode based on sensor input and user interaction, and communicates lighting data via **MQTT** to a remote broker.  

##  Circuit Connections

###  Breadboard Layout
The following diagram shows the physical wiring on the breadboard, illustrating how the VEML7700 light sensor and touch sensor are connected to the Arduino MKR WiFi 1010.
![Breadboard Diagram](421272c0-e44d-4248-93b1-0d114baa9a94.png)

###  Circuit Schematic
The schematic diagram below provides the logical wiring overview, clearly indicating signal, power, and ground connections between the components.
![Circuit Schematic](b9015982-5f20-4c00-991a-ea1022cf0450.png)

## Code Files Description

| File                  | Description                                                  |
| --------------------- | ------------------------------------------------------------ |
| **sketch_oct15a.ino** | The main Arduino program that controls the entire system. It reads data from the VEML7700 light sensor, detects input from the touch sensor, and switches between three lighting modes —Light Sensor, Lamp, and Party mode. The file also generates smooth LED color transitions and sends RGB data to the MQTT server for remote visualization. |
| **arduino_secrets.h** | This file stores all private credentials, including WiFi network names, passwords, and MQTT usernames/passwords. It keeps sensitive information separate from the main code to ensure network security and should not be uploaded to any public repository. |

##  Controller Mode Introduction

 The project runs in **three modes**, the default mode is controlled by VEML7700 light sensor, and the mode can switched by the **touch sensor**:

| Mode | Description | Behavior |
|------|--------------|-----------|
| **Light Sensor Mode** | Automatic mode | The Vespera's color changes smoothly with surrounding brightness (dark is purple → light is yellow). |
| **Lamp Mode** | Constant light | Steady warm-white lighting for reading or studying. |
| **Party Mode** | Dynamic mode | Random colorful flashing patterns for visual effects. |

## Final Work

### **Internal circuit and component layout** 

Inside the enclosure, the Arduino MKR WiFi 1010, VEML7700 light sensor, and touch sensor are mounted securely on a mini breadboard. The wiring follows the circuit schematic shown earlier.

<img src="E:\xwechat_files\wxid_bsl44snd0b1c22_e9d8\temp\RWTemp\2025-10\047665ea591e08fe7e61d096ec8c66be.jpg" alt="047665ea591e08fe7e61d096ec8c66be" style="zoom: 50%;" />

### External cardboard appearance display

The outer enclosure of the prototype is made from a hand-crafted cardboard box. It protects the circuit from external contact and gives the luminaire a clean, minimal appearance. A small window on one side allows the VEML7700 light sensor to detect ambient brightness, while another opening provides access to the touch sensor.

<img src="E:\xwechat_files\wxid_bsl44snd0b1c22_e9d8\temp\RWTemp\2025-10\0ed401ca0b138aaffc4fda34972ae88c.jpg" alt="0ed401ca0b138aaffc4fda34972ae88c" style="zoom:50%;" />
