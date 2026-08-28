# ESP32 Wi-Fi Walkie-Talkie

An **ESP32-based real-time Wi-Fi Walkie-Talkie** designed for voice communication using **UDP audio streaming**. The system uses an **INMP441 I2S digital microphone**, **MAX98357A I2S audio amplifier**, and a **Push-to-Talk (PTT)** control.

The hardware was designed in **KiCad**, including the schematic, PCB layout, and 3D visualization. The repository also contains the ESP32 Arduino source code and Gerber files for PCB manufacturing.

---

## Project Overview

This project is a wireless voice communication system based on two ESP32 boards communicating over Wi-Fi.

The voice signal is captured by the **INMP441 digital microphone** through the I2S interface. The ESP32 processes and transmits the audio data using **UDP over Wi-Fi**. At the receiving side, the audio is sent through I2S to the **MAX98357A audio amplifier**, which drives the speaker.

A **Push-to-Talk (PTT)** button is used to control voice transmission.

### Audio Communication Path

```text
Voice
  ↓
INMP441 I2S Microphone
  ↓
ESP32
  ↓
UDP Audio Streaming over Wi-Fi
  ↓
ESP32
  ↓
MAX98357A I2S Audio Amplifier
  ↓
Speaker
```

---

## Main Features

* ESP32-based wireless communication
* Real-time UDP audio streaming
* Wi-Fi-based voice communication
* I2S digital audio interface
* INMP441 I2S digital microphone
* MAX98357A I2S audio amplifier
* Push-to-Talk (PTT) control
* Custom PCB designed using KiCad
* Complete schematic design
* PCB layout design
* Front and back PCB layout
* 3D PCB visualization
* Gerber files for PCB manufacturing

---

## Hardware

| Component           | Function                                     |
| ------------------- | -------------------------------------------- |
| ESP32               | Main microcontroller and Wi-Fi communication |
| INMP441             | I2S digital microphone                       |
| MAX98357A           | I2S audio amplifier                          |
| Push-to-Talk Button | Controls voice transmission                  |
| Speaker             | Audio output                                 |
| Custom PCB          | Integrates the system hardware               |

---

## Software

The ESP32 firmware was developed using **Arduino IDE**.

The repository contains the final Arduino source code used for the ESP32 Walkie-Talkie system.

### Communication

* Wireless technology: **Wi-Fi**
* Communication protocol: **UDP**
* Audio interface: **I2S**

---

## PCB Design

The custom PCB was designed using **KiCad**.

The PCB design includes:

* Complete schematic
* Component placement
* PCB routing
* Front copper layout
* Back copper layout
* Power and signal routing
* 3D PCB visualization
* Gerber manufacturing files

---

## PCB Design Preview

### Schematic

![ESP32 Wi-Fi Walkie-Talkie Schematic](Images_4_ESP32-Walkie-Talkie/KiCad%20Schematic.JPG)

### PCB Front Layout

![PCB Front Layout](Images_4_ESP32-Walkie-Talkie/PCB%20Layout%20Front.JPG)

### PCB Back Layout

![PCB Back Layout](Images_4_ESP32-Walkie-Talkie/PCB%20Layout%20Back.JPG)

### 3D View

![PCB 3D View](Images_4_ESP32-Walkie-Talkie/3D%20View.JPG)

---

## Repository Contents

### KiCad Design Files

The `ESP32_Walkie-Talkie Files` folder contains the KiCad project files, including:

* KiCad project file
* KiCad schematic
* KiCad PCB layout
* Component/project data files

### Arduino Source Code

The `Final_Codes` folder contains the final ESP32 Arduino source code for the Walkie-Talkie system.

### Gerber Files

The `Gerber Files_4_ESP32 Walkie-Talkie` folder contains the generated Gerber files required for PCB manufacturing.

### Project Images

The `Images_4_ESP32-Walkie-Talkie` folder contains:

* Schematic image
* PCB front layout
* PCB back layout
* PCB 3D view

---

## Design Tools

* **KiCad** — Schematic and PCB design
* **Arduino IDE** — ESP32 firmware development

---

## Technical Highlights

**Microcontroller:** ESP32
**Wireless Communication:** Wi-Fi
**Communication Protocol:** UDP
**Audio Interface:** I2S
**Digital Microphone:** INMP441
**Audio Amplifier:** MAX98357A
**Control:** Push-to-Talk (PTT)

---

## Project Objective

The objective of this project was to develop an **ESP32-based wireless voice communication system** while combining embedded programming, Wi-Fi networking, real-time UDP audio streaming, digital I2S audio, schematic design, PCB layout, and PCB manufacturing file generation.

---

## Project Type

**ESP32 Embedded System | Wi-Fi Communication | Digital Audio | PCB Design**

**Design Software:** KiCad
**Programming Environment:** Arduino IDE
