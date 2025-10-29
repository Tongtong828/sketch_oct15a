#  Xintong Shao CASA0014 Coursework

##  Project Overview
This project is developed for the CASA0014 coursework.  
It demonstrates an **IoT-based adaptive lighting system** using the **Arduino MKR WiFi 1010**, a **VEML7700 light sensor**, and a **touch sensor**.  
The system dynamically changes its lighting mode based on sensor input and user interaction, and communicates lighting data via **MQTT** to a remote broker.  

##  Circuit Connections

###  Breadboard Layout
The following diagram shows the physical wiring on the breadboard, illustrating how the VEML7700 light sensor and touch sensor are connected to the Arduino MKR WiFi 1010.
<img width="905" height="1112" alt="Untitled Sketch 4_bb" src="https://github.com/user-attachments/assets/79bda03f-6baf-4477-abeb-4a0e9d8f6769" />


###  Circuit Schematic
The schematic diagram below provides the logical wiring overview, clearly indicating signal, power, and ground connections between the components.

<img width="906" height="514" alt="image" src="https://github.com/user-attachments/assets/69be4790-e0fe-43da-8585-f05685ade56b" />


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

### Effect  Simulation

To help visualize how the Vespera operates in different modes, a short demonstration video has been provided below.
 The video showcases how the light transitions between the three modes: Light Sensor Mode, Lamp Mode, and Party Mode，in real time.

 https://github.com/Tongtong828/sketch_oct15a/blob/main/Effect%20Simulation%20Test.mp4

## Final Work

### **Internal circuit and component layout** 

Inside the enclosure, the Arduino MKR WiFi 1010, VEML7700 light sensor, and touch sensor are mounted securely on a mini breadboard. The wiring follows the circuit schematic shown earlier.
![047665ea591e08fe7e61d096ec8c66be](https://github.com/user-attachments/assets/c3531a21-6a1d-4b01-8f92-5da7427420d9)



### External cardboard appearance display

The outer enclosure of the prototype is made from a hand-crafted cardboard box. It protects the circuit from external contact and gives the luminaire a clean, minimal appearance. A small window on one side allows the VEML7700 light sensor to detect ambient brightness, while another opening provides access to the touch sensor.

![1c6244ed2c8d900c523fa35a6fcaab25](https://github.com/user-attachments/assets/6516b3d2-6ed4-4e80-a2a1-9586fba9a374)

