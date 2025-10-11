/**
 * @file      uart.h
 * @brief     Public API for the portable UART driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions, data structures, and
 *            configuration options for interacting with a UART peripheral.
 *            It is designed to be hardware-agnostic and relies on a
 *            platform-specific porting layer for hardware access.
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing a UART driver instance.
 * @details The user only interacts with a pointer to this type and should
 *          never attempt to dereference it or access its members.
 */
typedef struct uart_handle_t* uart_handle_t;

/* --- Public Configuration and Callback Types --- */

/** @brief Enumeration of possible UART parity settings. */
typedef enum {
    UART_PARITY_NONE,  //!< No parity bit.
    UART_PARITY_EVEN,  //!< Even parity.
    UART_PARITY_ODD,   //!< Odd parity.
} uart_parity_t;

/** @brief Enumeration of possible UART stop bit settings. */
typedef enum {
    UART_STOP_BITS_1,    //!< 1 stop bit.
    UART_STOP_BITS_0_5,  //!< 0.5 stop bits (SmartCard mode).
    UART_STOP_BITS_2,    //!< 2 stop bits.
    UART_STOP_BITS_1_5,  //!< 1.5 stop bits (SmartCard mode).
} uart_stop_bits_t;

/** @brief Enumeration of possible UART flow control settings. */
typedef enum {
    UART_FLOW_CONTROL_NONE,     //!< No hardware flow control.
    UART_FLOW_CONTROL_RTS,      //!< RTS only.
    UART_FLOW_CONTROL_CTS,      //!< CTS only.
    UART_FLOW_CONTROL_RTS_CTS,  //!< RTS and CTS.
} uart_flow_control_t;

/**
 * @brief Configuration structure for UART initialization.
 * @details This structure is passed to uart_init() to configure a
 *          peripheral instance at runtime.
 */
typedef struct {
    uint32_t baud_rate;         //!< Desired baud rate in Hz (e.g., 115200).
    uint32_t word_length;       //!< Word length in bits (typically 8 or 9).
    uart_parity_t parity;       //!< Parity setting.
    uart_stop_bits_t stop_bits; //!< Stop bits setting.
    uart_flow_control_t flow_control; //!< Hardware flow control setting.
} uart_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes a UART peripheral instance.
 *
 * @details This function configures and enables a specific UART peripheral
 *          based on the provided configuration. It returns a handle that must
 *          be used for all subsequent operations on that instance.
 *
 * @param[in] instance_num The hardware instance number (e.g., 1 for USART1).
 * @param[in] config Pointer to the user-provided configuration structure.
 *                   The contents of this struct are copied into the driver's
 *                   internal state, so the original can be temporary.
 *
 * @return A handle to the initialized UART instance, or NULL on failure
 *         (e.g., invalid instance number or configuration).
 */
uart_handle_t uart_init(uint8_t instance_num, const uart_config_t* config);

/**
 * @brief De-initializes a UART peripheral instance.
 *
 * @details Disables the peripheral, its clocks, and releases any resources
 *          associated with the handle.
 *
 * @param[in,out] p_handle Pointer to the handle of the UART instance to de-initialize.
 *                         The handle will be set to NULL upon successful de-initialization.
 */
void uart_deinit(uart_handle_t* p_handle);

/**
 * @brief Writes a block of data to the UART in blocking mode.
 *
 * @details This function transmits a specified number of bytes over the UART.
 *          It will not return until all bytes have been successfully placed
 *          into the hardware transmit buffer.
 *
 * @param[in] handle The handle to the UART instance.
 * @param[in] p_data Pointer to the data buffer to transmit.
 * @param[in] len The number of bytes to transmit.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int uart_write_blocking(uart_handle_t handle, const uint8_t* p_data, size_t len);

/**
 * @brief Reads a block of data from the UART in blocking mode.
 *
 * @details This function receives a specified number of bytes from the UART.
 *          It will not return until the requested number of bytes has been
 *          received.
 *
 * @param[in] handle The handle to the UART instance.
 * @param[out] p_data Pointer to the buffer where received data will be stored.
 * @param[in] len The number of bytes to read.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int uart_read_blocking(uart_handle_t handle, uint8_t* p_data, size_t len);

#endif // UART_H
