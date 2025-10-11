/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Gemini
 * @brief          : Complete firmware for the STM32F103C8T6 Logic Analyzer
 ******************************************************************************
 * @attention
 *
 * This firmware is designed to pair with the provided Python UI. It uses
 * FreeRTOS, libopencm3, and a timer-triggered DMA to sample GPIO pins at 1 Msps.
 * It processes commands from the PC and sends back captured data in a
 * JSON format.
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For atoi

// libopencm3 headers
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

// FreeRTOS headers
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// --- Configuration Constants ---
#define F_CPU 72000000UL
#define SAMPLING_FREQUENCY_HZ 1000000 // 1 Msps
#define TIMER_PERIOD (F_CPU / SAMPLING_FREQUENCY_HZ)
#define DMA_BUFFER_SIZE 1024 // Total size of the circular buffer
#define DMA_BUFFER_HALF_SIZE (DMA_BUFFER_SIZE / 2)

// --- Task Configuration ---
#define COMM_TASK_STACK_SIZE     (configMINIMAL_STACK_SIZE + 256)
#define PROCESSING_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 768) // Larger for JSON formatting
#define COMM_TASK_PRIORITY       (tskIDLE_PRIORITY + 2)
#define PROCESSING_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

// --- Communication Buffers ---
#define UART_RX_BUFFER_SIZE 128
#define JSON_OUTPUT_BUFFER_SIZE 2048 // Large buffer to build the JSON response

// --- Global State & Data ---
typedef enum { IDLE, CAPTURING } AnalyzerState;
typedef enum { PROTO_GPIO, PROTO_SPI, PROTO_I2C, PROTO_UART } ProtocolType;

typedef struct {
    volatile AnalyzerState state;
    ProtocolType protocol;
    // Parameters for different protocols would go here
    struct {
        int cpol;
        int cpha;
    } spi_params;
} AnalyzerConfig;

static AnalyzerConfig analyzer_config = { .state = IDLE, .protocol = PROTO_GPIO };
static uint16_t dma_capture_buffer[DMA_BUFFER_SIZE];
static char json_output_buffer[JSON_OUTPUT_BUFFER_SIZE];

// --- RTOS Handles ---
static QueueHandle_t uart_rx_queue = NULL;
static QueueHandle_t json_output_queue = NULL;
static SemaphoreHandle_t dma_transfer_complete_sem = NULL;
volatile bool dma_first_half_done = false;

// --- Function Prototypes ---
static void clock_setup(void);
static void gpio_setup(void);
static void usart_setup(void);
static void timer_setup(void);
static void dma_setup(void);
void CommunicationTask(void *pvParameters);
void ProcessingTask(void *pvParameters);
static void process_gpio(uint16_t* data_buffer, uint32_t len, char* json_buffer, size_t json_buffer_size);
// Add prototypes for other protocol processors
// static void process_spi(...);

// --- Main Application ---
int main(void) {
    clock_setup();
    gpio_setup();
    dma_setup();
    usart_setup();
    timer_setup(); // Timer is started by a command

    // Create RTOS objects
    uart_rx_queue = xQueueCreate(1, UART_RX_BUFFER_SIZE);
    json_output_queue = xQueueCreate(1, sizeof(char*));
    dma_transfer_complete_sem = xSemaphoreCreateBinary();

    // Create Tasks
    xTaskCreate(CommunicationTask, "CommTask", COMM_TASK_STACK_SIZE, NULL, COMM_TASK_PRIORITY, NULL);
    xTaskCreate(ProcessingTask, "ProcTask", PROCESSING_TASK_STACK_SIZE, NULL, PROCESSING_TASK_PRIORITY, NULL);

    vTaskStartScheduler();
    for (;;); // Should never be reached
    return 0;
}

// --- Task Implementations ---

/**
 * @brief Handles all communication with the PC UI.
 * - Receives and parses JSON commands.
 * - Manages the analyzer's state (start/stop capture).
 * - Sends formatted JSON data back to the PC.
 */
void CommunicationTask(void *pvParameters) {
    (void)pvParameters;
    char rx_buffer[UART_RX_BUFFER_SIZE];
    char* json_to_send;

    for (;;) {
        // Check for an incoming command from the UART ISR
        if (xQueueReceive(uart_rx_queue, &rx_buffer, pdMS_TO_TICKS(10)) == pdTRUE) {
            // Simple string search for parsing JSON. For more complex JSON, use a library like jsmn.
            char *cmd_ptr = strstr(rx_buffer, "\"command\": \"");
            if (cmd_ptr) {
                cmd_ptr += strlen("\"command\": \"");
                if (strncmp(cmd_ptr, "configure", 9) == 0) {
                    char *proto_ptr = strstr(rx_buffer, "\"protocol\": \"");
                    if (proto_ptr) {
                        proto_ptr += strlen("\"protocol\": \"");
                        if (strncmp(proto_ptr, "GPIO", 4) == 0) analyzer_config.protocol = PROTO_GPIO;
                        else if (strncmp(proto_ptr, "SPI", 3) == 0) analyzer_config.protocol = PROTO_SPI;
                        // ... Parse other protocols and their parameters here
                    }
                } else if (strncmp(cmd_ptr, "start_capture", 13) == 0) {
                    if (analyzer_config.state == IDLE) {
                        analyzer_config.state = CAPTURING;
                        // Reset DMA and start the timer to begin capture
                        dma_set_memory_address(DMA1, DMA_CHANNEL2, (uint32_t)dma_capture_buffer);
                        dma_set_number_of_data(DMA1, DMA_CHANNEL2, DMA_BUFFER_SIZE);
                        dma_enable_channel(DMA1, DMA_CHANNEL2);
                        timer_enable_counter(TIM2);
                    }
                }
            }
        }

        // Check for a processed JSON buffer ready to be sent to the PC
        if (xQueueReceive(json_output_queue, &json_to_send, pdMS_TO_TICKS(10)) == pdTRUE) {
            char* ptr = json_to_send;
            while (*ptr) {
                usart_send_blocking(USART1, *ptr++);
            }
            usart_send_blocking(USART1, '\n'); // Terminator for readline() in Python
        }
    }
}

/**
 * @brief Processes the raw data captured by the DMA.
 * - Waits for a signal that a buffer half is ready.
 * - Calls the appropriate protocol decoder.
 * - Sends the formatted JSON string to the CommunicationTask.
 */
void ProcessingTask(void *pvParameters) {
    (void)pvParameters;

    for (;;) {
        // Wait for the DMA ISR to signal that a buffer half is full
        if (xSemaphoreTake(dma_transfer_complete_sem, portMAX_DELAY) == pdTRUE) {
            uint16_t* buffer_to_process;

            // Determine which half of the buffer to process
            if (dma_first_half_done) {
                buffer_to_process = &dma_capture_buffer[0];
            } else {
                buffer_to_process = &dma_capture_buffer[DMA_BUFFER_HALF_SIZE];
            }

            // Based on current config, call the correct processing function
            switch (analyzer_config.protocol) {
                case PROTO_GPIO:
                    process_gpio(buffer_to_process, DMA_BUFFER_HALF_SIZE, json_output_buffer, JSON_OUTPUT_BUFFER_SIZE);
                    break;
                case PROTO_SPI:
                    // process_spi(buffer_to_process, DMA_BUFFER_HALF_SIZE, ...);
                    // For now, just send a placeholder message
                    snprintf(json_output_buffer, JSON_OUTPUT_BUFFER_SIZE, "{\"log\":\"SPI decoder not implemented\"}");
                    break;
                // Add cases for I2C and UART
                default:
                    snprintf(json_output_buffer, JSON_OUTPUT_BUFFER_SIZE, "{\"log\":\"Unknown protocol selected\"}");
                    break;
            }

            // Send the pointer to the formatted JSON to the comm task
            char *json_ptr = json_output_buffer;
            xQueueSend(json_output_queue, &json_ptr, portMAX_DELAY);

            // Stop capturing after one buffer is processed.
            // For continuous capture, this logic would change.
            timer_disable_counter(TIM2);
            dma_disable_channel(DMA1, DMA_CHANNEL2);
            analyzer_config.state = IDLE;
        }
    }
}



// --- Helper Function for Safe JSON String Building ---

/**
 * @brief A wrapper for snprintf that updates the buffer pointer and remaining size.
 * @param buf Pointer to the current position in the character buffer.
 * @param remaining Pointer to the remaining size in the buffer.
 * @param format The format string for snprintf.
 * @param ... Variable arguments for the format string.
 */
static void safe_snprintf(char** buf, size_t* remaining, const char* format, ...) {
    if (*remaining <= 1) return;
    va_list args;
    va_start(args, format);
    int written = vsnprintf(*buf, *remaining, format, args);
    va_end(args);
    if (written > 0) {
        *buf += written;
        *remaining -= written;
    }
}

// --- Protocol Pin Mapping Assumptions ---
// These definitions map the protocol lines to the pins of GPIOB (PB0, PB1, etc.)

#define SPI_SCK_PIN     0
#define SPI_MOSI_PIN    1
#define SPI_MISO_PIN    2
#define SPI_CS_PIN      3

#define I2C_SCL_PIN     0
#define I2C_SDA_PIN     1

#define UART_RX_PIN     0


// --- SPI DECODER ---

/**
 * @brief Decodes SPI data (Mode 0: CPOL=0, CPHA=0) from raw samples.
 * Assumes pin mapping: SCK=PB0, MOSI=PB1, MISO=PB2, CS=PB3
 */
void process_spi(uint16_t* data_buffer, uint32_t len, char* json_buffer, size_t json_buffer_size) {
    char *ptr = json_buffer;
    size_t remaining = json_buffer_size;

    typedef enum { SPI_IDLE, SPI_TRANSFER } spi_state_t;
    spi_state_t state = SPI_IDLE;

    uint8_t mosi_byte = 0, miso_byte = 0;
    int bit_count = 0;
    uint32_t byte_start_timestamp = 0;

    // Start JSON object
    safe_snprintf(&ptr, &remaining, "{\"protocol\":\"SPI\",\"decoded\":[");
    bool first_decoded = true;

    for (uint32_t i = 1; i < len; i++) {
        // Get current and previous states of the relevant pins
        uint16_t prev_sample = data_buffer[i-1];
        uint16_t curr_sample = data_buffer[i];

        bool prev_cs  = (prev_sample >> SPI_CS_PIN)  & 0x01;
        bool curr_cs  = (curr_sample >> SPI_CS_PIN)  & 0x01;
        bool prev_sck = (prev_sample >> SPI_SCK_PIN) & 0x01;
        bool curr_sck = (curr_sample >> SPI_SCK_PIN) & 0x01;

        // State Transitions based on Chip Select (CS)
        if (prev_cs && !curr_cs) { // Falling edge on CS: Start of transaction
            state = SPI_TRANSFER;
            bit_count = 0;
        } else if (!prev_cs && curr_cs) { // Rising edge on CS: End of transaction
            state = SPI_IDLE;
        }

        if (state == SPI_TRANSFER) {
            // Check for active clock edge (rising edge for SPI Mode 0)
            if (!prev_sck && curr_sck) {
                if (bit_count == 0) {
                    byte_start_timestamp = i; // Timestamp of the first bit of a byte
                }

                // Sample MISO and MOSI on the active clock edge
                mosi_byte = (mosi_byte << 1) | ((curr_sample >> SPI_MOSI_PIN) & 0x01);
                miso_byte = (miso_byte << 1) | ((curr_sample >> SPI_MISO_PIN) & 0x01);
                bit_count++;

                if (bit_count == 8) {
                    // Full byte has been transferred
                    if (!first_decoded) safe_snprintf(&ptr, &remaining, ",");
                    safe_snprintf(&ptr, &remaining, "{\"ts\":%d,\"mosi\":%d,\"miso\":%d}", byte_start_timestamp, mosi_byte, miso_byte);
                    first_decoded = false;

                    // Reset for next byte
                    bit_count = 0;
                    mosi_byte = 0;
                    miso_byte = 0;
                }
            }
        }
    }

    // Close JSON object (waveform part is omitted for brevity but can be added like in process_gpio)
    safe_snprintf(&ptr, &remaining, "],\"waveform\":{}}");
}

static void process_spi(uint16_t* data_buffer, uint32_t len, char* json_buffer, size_t json_buffer_size);
static void process_i2c(uint16_t* data_buffer, uint32_t len, char* json_buffer, size_t json_buffer_size);
static void process_uart(uint16_t* data_buffer, uint32_t len, char* json_buffer, size_t json_buffer_size);

// This is the updated switch block for your ProcessingTask
// ... inside ProcessingTask, after xSemaphoreTake ...

switch (analyzer_config.protocol) {
    case PROTO_GPIO:
        process_gpio(buffer_to_process, DMA_BUFFER_HALF_SIZE, json_output_buffer, JSON_OUTPUT_BUFFER_SIZE);
        break;
    case PROTO_SPI:
        process_spi(buffer_to_process, DMA_BUFFER_HALF_SIZE, json_output_buffer, JSON_OUTPUT_BUFFER_SIZE);
        break;
    case PROTO_I2C:
        process_i2c(buffer_to_process, DMA_BUFFER_HALF_SIZE, json_output_buffer, JSON_OUTPUT_BUFFER_SIZE);
        break;
    case PROTO_UART:
        process_uart(buffer_to_process, DMA_BUFFER_HALF_SIZE, json_output_buffer, JSON_OUTPUT_BUFFER_SIZE);
        break;
    default:
        snprintf(json_output_buffer, JSON_OUTPUT_BUFFER_SIZE, "{\"log\":\"Unknown protocol selected\"}");
        break;
}

// ... the rest of the task remains the same (xQueueSend, etc.) ...


// --- I2C DECODER ---

/**
 * @brief Decodes I2C data from raw samples.
 * Assumes pin mapping: SCL=PB0, SDA=PB1
 */
void process_i2c(uint16_t* data_buffer, uint32_t len, char* json_buffer, size_t json_buffer_size) {
    char *ptr = json_buffer;
    size_t remaining = json_buffer_size;

    typedef enum { I2C_IDLE, I2C_DATA, I2C_ACK } i2c_state_t;
    i2c_state_t state = I2C_IDLE;

    uint8_t current_byte = 0;
    int bit_count = 0;

    safe_snprintf(&ptr, &remaining, "{\"protocol\":\"I2C\",\"decoded\":[");
    bool first_decoded = true;

    for (uint32_t i = 1; i < len; i++) {
        uint16_t prev_sample = data_buffer[i-1];
        uint16_t curr_sample = data_buffer[i];

        bool prev_scl = (prev_sample >> I2C_SCL_PIN) & 0x01;
        bool curr_scl = (curr_sample >> I2C_SCL_PIN) & 0x01;
        bool prev_sda = (prev_sample >> I2C_SDA_PIN) & 0x01;
        bool curr_sda = (curr_sample >> I2C_SDA_PIN) & 0x01;

        // --- Detect Start and Stop Conditions (can happen anytime) ---
        // START: SDA falls while SCL is high
        if (curr_scl && prev_scl && prev_sda && !curr_sda) {
            state = I2C_DATA;
            bit_count = 0;
            current_byte = 0;
            if (!first_decoded) safe_snprintf(&ptr, &remaining, ",");
            safe_snprintf(&ptr, &remaining, "{\"ts\":%d,\"type\":\"START\"}", i);
            first_decoded = false;
        }
        // STOP: SDA rises while SCL is high
        else if (curr_scl && prev_scl && !prev_sda && curr_sda) {
            state = I2C_IDLE;
            if (!first_decoded) safe_snprintf(&ptr, &remaining, ",");
            safe_snprintf(&ptr, &remaining, "{\"ts\":%d,\"type\":\"STOP\"}", i);
            first_decoded = false;
        }

        // --- Clock-dependent logic: sample on rising edge of SCL ---
        if (!prev_scl && curr_scl) {
            if (state == I2C_DATA) {
                current_byte = (current_byte << 1) | curr_sda;
                bit_count++;
                if (bit_count == 8) {
                    if (!first_decoded) safe_snprintf(&ptr, &remaining, ",");
                    safe_snprintf(&ptr, &remaining, "{\"ts\":%d,\"type\":\"DATA\",\"value\":%d}", i, current_byte);
                    first_decoded = false;
                    state = I2C_ACK;
                    bit_count = 0;
                }
            } else if (state == I2C_ACK) {
                if (!first_decoded) safe_snprintf(&ptr, &remaining, ",");
                safe_snprintf(&ptr, &remaining, "{\"ts\":%d,\"type\":\"ACK\",\"ack\":%d}", i, !curr_sda);
                first_decoded = false;
                state = I2C_DATA;
            }
        }
    }
    safe_snprintf(&ptr, &remaining, "],\"waveform\":{}}");
}


// --- UART DECODER ---

/**
 * @brief Decodes UART data from raw samples.
 * Assumes pin mapping: RX=PB0. Assumes 9600 baud, 8-N-1 format.
 */
void process_uart(uint16_t* data_buffer, uint32_t len, char* json_buffer, size_t json_buffer_size) {
    char *ptr = json_buffer;
    size_t remaining = json_buffer_size;

    // NOTE: Baud rate should ideally come from analyzer_config
    const uint32_t baud_rate = 9600;
    const uint32_t samples_per_bit = SAMPLING_FREQUENCY_HZ / baud_rate;
    const uint32_t sample_point_offset = samples_per_bit / 2;

    typedef enum { UART_IDLE, UART_DATA, UART_STOP } uart_state_t;
    uart_state_t state = UART_IDLE;

    uint8_t current_byte = 0;
    int bit_count = 0;
    uint32_t next_sample_point = 0;

    safe_snprintf(&ptr, &remaining, "{\"protocol\":\"UART\",\"decoded\":[");
    bool first_decoded = true;

    for (uint32_t i = 1; i < len; i++) {
        bool prev_rx = (data_buffer[i-1] >> UART_RX_PIN) & 0x01;
        bool curr_rx = (data_buffer[i] >> UART_RX_PIN) & 0x01;

        if (state == UART_IDLE) {
            // Look for a falling edge (start bit)
            if (prev_rx && !curr_rx) {
                // Found start bit, calculate the first data bit sample point
                next_sample_point = i + samples_per_bit + sample_point_offset;
                state = UART_DATA;
                current_byte = 0;
                bit_count = 0;
            }
        }
        else if (i == next_sample_point) {
            if (state == UART_DATA) {
                // Sample the data bit
                current_byte |= (curr_rx << bit_count);
                bit_count++;
                // Set next sample point
                next_sample_point += samples_per_bit;
                if (bit_count == 8) {
                    state = UART_STOP; // Move to check for stop bit
                }
            }
            else if (state == UART_STOP) {
                bool error = !curr_rx; // Stop bit should be high (1)

                if (!first_decoded) safe_snprintf(&ptr, &remaining, ",");
                safe_snprintf(&ptr, &remaining, "{\"ts\":%d,\"value\":%d,\"error\":\"%s\"}", i, current_byte, error ? "Framing" : "None");
                first_decoded = false;

                state = UART_IDLE; // Go back to looking for a start bit
            }
        }
    }
    safe_snprintf(&ptr, &remaining, "],\"waveform\":{}}");
}


// --- Protocol Processing Functions ---

/**
 * @brief Decodes GPIO data and formats it into JSON.
 * This is a sample implementation that finds state changes.
 */
static void process_gpio(uint16_t* data_buffer, uint32_t len, char* json_buffer, size_t json_buffer_size) {
    char *ptr = json_buffer;
    size_t remaining = json_buffer_size;
    int written;

    // Start JSON object
    written = snprintf(ptr, remaining, "{\"protocol\":\"GPIO\",\"decoded\":[],\"waveform\":{");
    ptr += written; remaining -= written;

    uint16_t last_states[8] = {0xFFFF}; // Initialize to invalid state
    bool first_channel = true;

    // Iterate through each of the 8 channels (PB0-PB7)
    for (uint8_t ch = 0; ch < 8; ch++) {
        bool channel_has_data = false;
        char temp_waveform_buffer[512] = {0}; // Buffer for this channel's waveform
        char* wf_ptr = temp_waveform_buffer;

        uint16_t last_state = (data_buffer[0] >> ch) & 0x01;
        last_states[ch] = last_state;

        // Add the initial state
        int wf_written = snprintf(wf_ptr, sizeof(temp_waveform_buffer), "[[0,%d]", last_state);
        wf_ptr += wf_written;

        // Find all transitions for this channel in the buffer
        for (uint32_t i = 1; i < len; i++) {
            uint16_t current_state = (data_buffer[i] >> ch) & 0x01;
            if (current_state != last_state) {
                channel_has_data = true;
                wf_written = snprintf(wf_ptr, sizeof(temp_waveform_buffer) - (wf_ptr - temp_waveform_buffer), ",[%d,%d]", i, current_state);
                wf_ptr += wf_written;
                last_state = current_state;
            }
        }
        strcat(wf_ptr, "]"); // Close the waveform array

        // If there was any data, add this channel to the JSON
        if(channel_has_data) {
            if (!first_channel) {
                written = snprintf(ptr, remaining, ",");
                ptr += written; remaining -= written;
            }
            written = snprintf(ptr, remaining, "\"ch%d\":%s", ch, temp_waveform_buffer);
            ptr += written; remaining -= written;
            first_channel = false;
        }
    }

    // Close JSON object
    snprintf(ptr, remaining, "}}");
}


// --- ISRs ---

void dma1_channel2_isr(void) {
    BaseType_t higher_priority_task_woken = pdFALSE;

    if (dma_get_interrupt_flag(DMA1, DMA_CHANNEL2, DMA_HTIF)) {
        dma_clear_interrupt_flags(DMA1, DMA_CHANNEL2, DMA_HTIF);
        dma_first_half_done = true;
        xSemaphoreGiveFromISR(dma_transfer_complete_sem, &higher_priority_task_woken);
    }

    if (dma_get_interrupt_flag(DMA1, DMA_CHANNEL2, DMA_TCIF)) {
        dma_clear_interrupt_flags(DMA1, DMA_CHANNEL2, DMA_TCIF);
        dma_first_half_done = false;
        xSemaphoreGiveFromISR(dma_transfer_complete_sem, &higher_priority_task_woken);
    }
    portYIELD_FROM_ISR(higher_priority_task_woken);
}

void usart1_isr(void) {
    static char rx_buffer[UART_RX_BUFFER_SIZE];
    static uint8_t rx_index = 0;

    if (usart_get_flag(USART1, USART_SR_RXNE)) {
        char received_char = usart_recv(USART1);
        if (received_char == '\n' || received_char == '\r') {
            if (rx_index > 0) {
                rx_buffer[rx_index] = '\0';
                // Send the completed string to the queue
                xQueueSendFromISR(uart_rx_queue, &rx_buffer, NULL);
                rx_index = 0;
            }
        } else {
            if (rx_index < (UART_RX_BUFFER_SIZE - 1)) {
                rx_buffer[rx_index++] = received_char;
            }
        }
    }
}

// --- Hardware Setup Functions ---

static void clock_setup(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_DMA1);
}

static void gpio_setup(void) {
    // UART Pins
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX); // PA9
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX); // PA10

    // Logic Analyzer Input Pins (PB0-PB7)
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7);
}

static void usart_setup(void) {
    nvic_enable_irq(NVIC_USART1_IRQ);
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_enable_rx_interrupt(USART1);
    usart_enable(USART1);
}

static void timer_setup(void) {
    timer_reset(TIM2);
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 0); // 72MHz / (0+1) = 72MHz clock
    timer_set_period(TIM2, TIMER_PERIOD - 1); // 72MHz / 72 = 1MHz update rate
    timer_enable_irq(TIM2, TIM_DIER_UDE); // Enable DMA request on update
    // Do not enable the counter here; it will be enabled by a command
}

static void dma_setup(void) {
    nvic_enable_irq(NVIC_DMA1_CHANNEL2_IRQ);
    dma_channel_reset(DMA1, DMA_CHANNEL2);
    dma_set_peripheral_address(DMA1, DMA_CHANNEL2, (uint32_t)&GPIOB_IDR);
    dma_set_memory_address(DMA1, DMA_CHANNEL2, (uint32_t)dma_capture_buffer);
    dma_set_number_of_data(DMA1, DMA_CHANNEL2, DMA_BUFFER_SIZE);
    dma_set_read_from_peripheral(DMA1, DMA_CHANNEL2);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL2);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL2, DMA_CCR_PSIZE_16BIT);
    dma_set_memory_size(DMA1, DMA_CHANNEL2, DMA_CCR_MSIZE_16BIT);
    dma_enable_circular_mode(DMA1, DMA_CHANNEL2);
    dma_set_priority(DMA1, DMA_CHANNEL2, DMA_CCR_PL_VERY_HIGH);
    dma_enable_half_transfer_interrupt(DMA1, DMA_CHANNEL2);
    dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL2);
    // Do not enable the channel here; it will be enabled by a command
}

