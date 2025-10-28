#  CASA0014 Coursework

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

##  System Operation

 The project runs in **three modes**, the default mode is controled by VEML7700 light sensor, and the mode can switched by the **touch sensor**:

| Mode | Description | Behavior |
|------|--------------|-----------|
| **Light Sensor Mode** | Automatic mode | The Vespera's color changes smoothly with surrounding brightness (dark is purple → light is yellow). |
| **Lamp Mode** | Constant light | Steady warm-white lighting for reading or studying. |
| **Party Mode** | Dynamic mode | Random colorful flashing patterns for visual effects. |

---
