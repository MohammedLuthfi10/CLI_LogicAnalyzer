# 🔍 STM32-Based Mini Logic Analyzer

An **open-source, multi-protocol logic analyzer** built using the **STM32F103C8T6 "Blue Pill"** microcontroller and a **Python-based PC user interface**.  
Designed for hobbyists, students, and embedded developers, this tool provides **real-time digital signal capture**, **protocol decoding**, and **live waveform visualization** — all at an affordable cost.

---

## ⚙️ Features

- ⚡ **High-Speed Sampling:** Up to **1 Msps** across **8 channels**, using **timer-triggered DMA** for precise timing.  
- 🧩 **Multi-Protocol Decoding:**
  - **SPI** (Serial Peripheral Interface)
  - **I2C** (Inter-Integrated Circuit)
  - **UART** (Universal Asynchronous Receiver-Transmitter)
  - **GPIO** (State Change Detection)
- 📈 **Real-Time Visualization:** Python GUI with live waveform plotting and decoded data logs.
- 🧠 **Configurable Parameters:** On-the-fly protocol setup from the PC app (e.g., SPI mode, I2C address).
- 🕹️ **FreeRTOS-Based Firmware:** Concurrent and efficient real-time operation.
- 💻 **Cross-Platform UI:** Python/Tkinter GUI works on **Windows**, **macOS**, and **Linux**.
- 💰 **Affordable & Open Source:** Built using low-cost hardware and free software.

---

## 🧠 System Architecture

### 🖥️ STM32 Firmware

Built with **FreeRTOS** and structured into concurrent tasks for non-blocking operation.

| Task | Description |
|------|--------------|
| **Communication Task** | Manages UART link with the PC. Parses incoming JSON commands (e.g., `configure`, `start_capture`) and sends formatted JSON packets back. |
| **Processing Task** | Waits for DMA buffer fills, decodes protocol data (SPI/I2C/UART), and sends results as JSON strings. |
| **Acquisition (DMA/ISR)** | Uses **TIM2** at 1 MHz to trigger DMA, sampling **GPIOB** pins into a circular buffer. Interrupts notify the Processing Task when half/full buffer events occur. |

---

### 🧰 PC Application

Developed in **Python** using:
- **Tkinter** → GUI  
- **Matplotlib** → Waveform plotting  
- **PySerial** → Serial communication  

| Component | Description |
|------------|-------------|
| **Main UI Thread** | Handles user interactions (buttons, selections) and updates the waveform and decoded logs. |
| **Serial Reader Thread** | Continuously reads data from STM32 and stores it in a thread-safe queue. |
| **Queue Processor** | Periodically checks for new messages and updates the GUI in real-time. |

---

## 🧩 Hardware & Software Requirements

### 🛠️ Hardware

| Component | Purpose |
|------------|----------|
| **STM32F103C8T6 “Blue Pill”** | Core MCU for data acquisition |
| **USB-to-UART Converter (CP2102 / CH340)** | Communication between STM32 and PC |
| **ST-Link V2** | Programming/debugging the STM32 |
| **Jumper Wires** | For connecting STM32 to the target device |

---

### 💻 Software

#### Firmware Development
- [ARM GCC Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain)  
- `make` build utility  
- [libopencm3](https://github.com/libopencm3/libopencm3) library  
- [FreeRTOS](https://www.freertos.org/) source  

#### PC Application
- Python 3.6+
- Required libraries:
  ```bash
  pip install pyserial matplotlib
