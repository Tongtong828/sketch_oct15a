#  CASA0014 Coursework

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

<img width="885" height="485" alt="image" src="https://github.com/user-attachments/assets/5a114028-6ffe-4e36-bb1e-0431614e3351" />


##  System Operation

 The project runs in **three modes**, the default mode is controled by VEML7700 light sensor, and the mode can switched by the **touch sensor**:

| Mode | Description | Behavior |
|------|--------------|-----------|
| **Light Sensor Mode** | Automatic mode | The Vespera's color changes smoothly with surrounding brightness (dark is purple → light is yellow). |
| **Lamp Mode** | Constant light | Steady warm-white lighting for reading or studying. |
| **Party Mode** | Dynamic mode | Random colorful flashing patterns for visual effects. |

---
