STM32-Based Mini Logic Analyzer

An open-source, multi-protocol logic analyzer built with an STM32F103C8T6 "Blue Pill" microcontroller and a Python-based PC user interface. This project is designed to be a powerful and affordable tool for hobbyists, students, and embedded systems developers to debug and analyze digital communication protocols.

The system uses a timer-triggered DMA on the STM32 to achieve a high-speed sampling rate of 1 Msps (1 million samples per second) across 8 channels. Data is processed in real-time using FreeRTOS and sent to a PC application for live visualization of waveforms and decoded protocol messages.

Features

High-Speed Sampling: Captures digital signals on up to 8 channels at 1 Msps using timer-driven DMA for precision.

Multi-Protocol Decoding: Built-in software decoders for common protocols:

SPI (Serial Peripheral Interface)

I2C (Inter-Integrated Circuit)

UART (Universal Asynchronous Receiver-Transmitter)

General Purpose GPIO (State change detection)

Real-Time Visualization: A user-friendly Python GUI displays captured digital waveforms and a log of decoded data packets.

Configurable Parameters: The PC application allows for on-the-fly configuration of protocol parameters (e.g., SPI mode, I2C address to watch).

Robust Firmware: Built on the FreeRTOS real-time operating system, ensuring efficient and concurrent handling of data acquisition, processing, and communication tasks.

Cross-Platform UI: The Python/Tkinter-based GUI is easy to run on Windows, macOS, and Linux.

Affordable and Accessible: Built entirely with low-cost, readily available development boards and open-source software.

System Architecture

The system is split into two main components: the STM32 firmware for data acquisition and the Python application for control and visualization.

Firmware Architecture (STM32)

The firmware runs on FreeRTOS and is organized into three concurrent tasks to ensure non-blocking operation:

Communication Task: Manages the UART link with the PC. It parses incoming JSON commands (e.g., configure, start_capture) and sends fully formatted JSON data packets back to the UI.

Processing Task: The core of the analyzer. It waits for a buffer of raw samples from the DMA, then passes it to the appropriate protocol decoder (SPI, I2C, etc.). After decoding, it formats the waveform and decoded messages into a JSON string.

Acquisition (DMA/ISR): This is not a task but a high-priority interrupt-driven process. TIM2 is configured to trigger at 1 MHz. Each trigger signals the DMA controller to copy the state of the GPIOB input pins into a circular buffer without any CPU intervention. When a buffer half is full, an interrupt is triggered, signaling the Processing Task to begin its work.

PC UI Architecture (Python)

The PC application is built with Tkinter for the GUI, Matplotlib for plotting, and PySerial for communication.

Main UI Thread: Handles all user interactions (button clicks, dropdown selections) and updates the waveform plot and text logs.

Serial Reader Thread: Runs in the background to continuously listen for incoming data from the STM32. This prevents the UI from freezing while waiting for data. It places received messages into a thread-safe queue.

Queue Processor: A function running on the main UI timer periodically checks the queue for new messages from the serial thread and updates the display accordingly.

Hardware and Software Requirements

Hardware

STM32F103C8T6 "Blue Pill" Board: The core microcontroller for data acquisition.

USB-to-UART Converter: A model like CP2102 or CH340 to communicate between the STM32 and the PC.

ST-Link V2 Programmer: (Recommended) For flashing the firmware onto the STM32.

Jumper Wires: For connecting the STM32 to the target device.

Software

Firmware Development:

ARM GCC Toolchain (arm-none-eabi-gcc).

make build utility.

libopencm3 library for STM32 peripherals.

FreeRTOS source files.

PC Application:

Python 3.6+

Required Python libraries. Install them via pip:

pip install pyserial matplotlib


Pinout and Connections

The logic analyzer uses pins PB0-PB7 as its 8 input channels. Connect the STM32 to your target device according to the protocol you wish to analyze.

Protocol

Signal

STM32 Pin

SPI

SCK

PB0



MOSI

PB1



MISO

PB2



CS

PB3

I2C

SCL

PB0



SDA

PB1

UART

RX

PB0

GPIO

CH0

PB0



...

...



CH7

PB7

USB-to-UART Connection:

STM32 PA9 (TX) -> Converter RX

STM32 PA10 (RX) -> Converter TX

STM32 GND -> Converter GND

STM32 5V -> Converter 5V (to power the board)

How to Build and Run

1. Firmware Setup

Clone the repository:

git clone [https://github.com/your-username/stm32-logic-analyzer.git](https://github.com/your-username/stm32-logic-analyzer.git)
cd stm32-logic-analyzer/firmware


Set up Toolchain: Ensure you have the ARM GCC toolchain and make installed and in your system's PATH.

Libraries: Place the libopencm3 and FreeRTOS source directories in the designated lib/ folder or adjust the Makefile to point to their locations.

Compile: Run the make command in the firmware/ directory.

make


Flash: Use an ST-Link V2 and a tool like st-flash to write the compiled binary to the STM32.

st-flash write build/logic_analyzer.bin 0x8000000


2. PC Application

Navigate to the UI directory:

cd ../ui


Install Dependencies:

pip install -r requirements.txt


Run the application:

python logic_analyzer_ui.py
