/*
 * uart.h
 *
 *  Created on: Aug 6, 2025
 *      Author: hp
 */

#ifndef UART_H_
#define UART_H_

// --- Opaque Handle Definition ---
/**
 * @brief Opaque handle representing a UART driver instance.
 * @details The user only interacts with a pointer to this type and should
 *          never attempt to dereference it or access its members.
 */
typedef struct uart_handle_t* uart_handle_t;


// --- Public Configuration and Callback Types ---

/**
 * @brief Callback function type for received data.
 * @param[in] byte The byte of data received.
 * @param[in] user_data A pointer to user-defined context, passed during init.
 */
typedef void (*uart_rx_callback_t)(uint8_t byte, void* user_data);

/**
 * @brief Configuration structure for UART initialization.
 */
typedef struct {
    uint32_t baud_rate;
    uint32_t databits;
    uint32_t stopbits;
    uint32_t parity;
    uint32_t mode;
    uint32_t flowcontrol;

    // Optional callbacks for asynchronous operation
    uart_rx_callback_t rx_callback;
    void* callback_user_data; // User context for callbacks

} uart_config_t;

// --- Public API Functions ---

/**
 * @brief Initializes a UART peripheral instance.
 * @param[in] instance_num The hardware instance number (e.g., 1 for UART1).
 * @param[in] config Pointer to the user-provided configuration structure.
 * @return A handle to the initialized UART instance, or NULL on failure.
 */
uart_handle_t uart_init(uint8_t instance_num, const uart_config_t* config);

/**
 * @brief De-initializes a UART peripheral instance.
 * @param[in] handle The handle to the UART instance to de-initialize.
 */
void uart_deinit(uart_handle_t handle);

/**
 * @brief Writes a block of data to the UART in blocking mode.
 * @param[in] handle The handle to the UART instance.
 * @param[in] data Pointer to the data buffer to transmit.
 * @param[in] len The number of bytes to transmit.
 * @return 0 on success, or a negative error code on failure.
 */
int uart_write_blocking(uart_handle_t handle, const uint8_t* data, size_t len);

//... other public functions (read, get_status, etc.)

#endif /* UART_H_ */
