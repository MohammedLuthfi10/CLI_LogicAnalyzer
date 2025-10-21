# STM32-Based Mini Logic Analyzer

![Project Banner](https://placehold.co/1200x400/4F46E5/FFFFFF?text=STM32%20Mini%20Logic%20Analyzer&font=inter)

An open-source, multi-protocol logic analyzer built with an STM32F103C8T6 "Blue Pill" microcontroller and a Python-based PC user interface. This project is designed to be a powerful and affordable tool for hobbyists, students, and embedded systems developers to debug and analyze digital communication protocols.

The system uses a timer-triggered DMA on the STM32 to achieve a high-speed sampling rate of 1 Msps (1 million samples per second) across 8 channels. Data is processed in real-time using FreeRTOS and sent to a PC application for live visualization of waveforms and decoded protocol messages.

---

## Features

* **High-Speed Sampling:** Captures digital signals on up to 8 channels at 1 Msps using timer-driven DMA for precision.
* **Multi-Protocol Decoding:** Built-in software decoders for common protocols:
    * **SPI** (Serial Peripheral Interface)
    * **I2C** (Inter-Integrated Circuit)
    * **UART** (Universal Asynchronous Receiver-Transmitter)
    * **General Purpose GPIO** (State change detection)
* **Real-Time Visualization:** A user-friendly Python GUI displays captured digital waveforms and a log of decoded data packets.
* **Configurable Parameters:** The PC application allows for on-the-fly configuration of protocol parameters (e.g., SPI mode, I2C address to watch).
* **Robust Firmware:** Built on the FreeRTOS real-time operating system, ensuring efficient and concurrent handling of data acquisition, processing, and communication tasks.
* **Cross-Platform UI:** The Python/Tkinter-based GUI is easy to run on Windows, macOS, and Linux.
* **Affordable and Accessible:** Built entirely with low-cost, readily available development boards and open-source software.

---

## System Architecture

The system is split into two main components: the STM32 firmware for data acquisition and the Python application for control and visualization.



### Firmware Architecture (`STM32`)

The firmware runs on FreeRTOS and is organized into three concurrent tasks to ensure non-blocking operation:

1.  **Communication Task:** Manages the UART link with the PC. It parses incoming JSON commands (e.g., `configure`, `start_capture`) and sends fully formatted JSON data packets back to the UI.
2.  **Processing Task:** The core of the analyzer. It waits for a buffer of raw samples from the DMA, then passes it to the appropriate protocol decoder (SPI, I2C, etc.). After decoding, it formats the waveform and decoded messages into a JSON string.
3.  **Acquisition (DMA/ISR):** This is not a task but a high-priority interrupt-driven process. `TIM2` is configured to trigger at 1 MHz. Each trigger signals the DMA controller to copy the state of the `GPIOB` input pins into a circular buffer without any CPU intervention. When a buffer half is full, an interrupt is triggered, signaling the Processing Task to begin its work.

### PC UI Architecture (`Python`)

The PC application is built with Tkinter for the GUI, Matplotlib for plotting, and PySerial for communication.

1.  **Main UI Thread:** Handles all user interactions (button clicks, dropdown selections) and updates the waveform plot and text logs.
2.  **Serial Reader Thread:** Runs in the background to continuously listen for incoming data from the STM32. This prevents the UI from freezing while waiting for data. It places received messages into a thread-safe queue.
3.  **Queue Processor:** A function running on the main UI timer periodically checks the queue for new messages from the serial thread and updates the display accordingly.

---

## Hardware and Software Requirements

### Hardware
* **STM32F103C8T6 "Blue Pill" Board:** The core microcontroller for data acquisition.
* **USB-to-UART Converter:** A model like CP2102 or CH340 to communicate between the STM32 and the PC.
* **ST-Link V2 Programmer:** (Recommended) For flashing the firmware onto the STM32.
* **Jumper Wires:** For connecting the STM32 to the target device.

### Software
* **Firmware Development:**
    * ARM GCC Toolchain (`arm-none-eabi-gcc`).
    * `make` build utility.
    * [libopencm3](https://github.com/libopencm3/libopencm3) library for STM32 peripherals.
    * FreeRTOS source files.
* **PC Application:**
    * Python 3.6+
    * Required Python libraries. Install them via pip:
        ```bash
        pip install pyserial matplotlib
        ```
---

## Pinout and Connections

The logic analyzer uses pins **PB0-PB7** as its 8 input channels. Connect the STM32 to your target device according to the protocol you wish to analyze.

| Protocol | Signal | STM32 Pin |
| :------- | :----- | :-------- |
| **SPI** | `SCK`  | `PB0`     |
|          | `MOSI` | `PB1`     |
|          | `MISO` | `PB2`     |
|          | `CS`   | `PB3`     |
| **I2C** | `SCL`  | `PB0`     |
|          | `SDA`  | `PB1`     |
| **UART** | `RX`   | `PB0`     |
| **GPIO** | `CH0`  | `PB0`     |
|          | `...`  | `...`     |
|          | `CH7`  | `PB7`     |

**USB-to-UART Connection:**
* `STM32 PA9 (TX)` -> `Converter RX`
* `STM32 PA10 (RX)` -> `Converter TX`
* `STM32 GND` -> `Converter GND`
* `STM32 5V` -> `Converter 5V` (to power the board)

---

## How to Build and Run

### 1. Firmware Setup
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/your-username/stm32-logic-analyzer.git](https://github.com/your-username/stm32-logic-analyzer.git)
    cd stm32-logic-analyzer/firmware
    ```
2.  **Set up Toolchain:** Ensure you have the ARM GCC toolchain and `make` installed and in your system's PATH.
3.  **Libraries:** Place the `libopencm3` and `FreeRTOS` source directories in the designated `lib/` folder or adjust the `Makefile` to point to their locations.
4.  **Compile:** Run the `make` command in the `firmware/` directory.
    ```bash
    make
    ```
5.  **Flash:** Use an ST-Link V2 and a tool like `st-flash` to write the compiled binary to the STM32.
    ```bash
    st-flash write build/logic_analyzer.bin 0x8000000
    ```

### 2. PC Application
1.  **Navigate to the UI directory:**
    ```bash
    cd ../ui
    ```
2.  **Install Dependencies:**
    ```bash
    pip install -r requirements.txt
    ```
3.  **Run the application:**
    ```bash
    python logic_analyzer_ui.py
    ```
![Screenshot of the Logic Analyzer UI](https://i.imgur.com/uG9jS7v.png)

---

## Usage Guide
1.  **Connect Hardware:** Wire the STM32's input pins (PB0-PB7) to the signal lines of the device you want to test.
2.  **Power On:** Connect the STM32 to your computer via the USB-to-UART converter.
3.  **Launch UI:** Run the Python application.
4.  **Connect to Device:** Select the correct COM port from the dropdown menu in the UI and click **"Connect"**.
5.  **Configure Capture:** Choose the protocol you want to analyze from the "Protocol" dropdown. The relevant configuration options will appear.
6.  **Start Capture:** Click the **"Start Capture"** button. The STM32 will arm itself, capture the next burst of data, process it, and send the results back.
7.  **Analyze Results:** The waveform will be displayed in the plot, and the decoded data (e.g., I2C addresses, SPI data bytes) will appear in the "Decoded Log".

---

## Future Improvements

This project provides a solid foundation that can be extended with more advanced features:

-   [ ] **Advanced Triggers:** Implement triggers to start capturing on specific events (e.g., rising/falling edge, specific data pattern).
-   [ ] **More Protocol Decoders:** Add support for protocols like CAN, 1-Wire, or WS2812B.
-   [ ] **Continuous Streaming:** Modify the firmware and UI to support a continuous data stream instead of single-shot captures.
-   [ ] **Data Export:** Add a feature to save captured waveforms and decoded logs to a file (e.g., CSV, VCD).
-   [ ] **UI Enhancements:** Add measurement tools, protocol-layer coloring, and search functionality to the UI.

---

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
