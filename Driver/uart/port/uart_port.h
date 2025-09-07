/*
 * uart_port.h
 *
 *  Created on: Aug 6, 2025
 *      Author: hp
 */

#ifndef DRIVER_UART_PORT_UART_PORT_H_
#define DRIVER_UART_PORT_UART_PORT_H_


// Forward declare the handle struct to avoid circular dependencies.
// The full definition is in uart_private.h, which port implementations will include.
struct uart_handle_t;

/**
 * @brief A structure of function pointers that defines the hardware-dependent
 *        operations required by the UART driver.
 */
typedef struct {
    /** @brief Enables the peripheral clock. */
    void (*enable_clock)(struct uart_handle_t* handle);

    /** @brief Disables the peripheral clock. */
    void (*disable_clock)(struct uart_handle_t* handle);

    /** @brief Initializes the GPIO pins for UART function. */
    void (*init_pins)(struct uart_handle_t* handle);

    /** @brief Configures the core UART settings (baud, parity, etc.). */
    void (*configure_core)(struct uart_handle_t* handle);

    /** @brief Writes a single byte, blocking until complete. */
    void (*write_byte_blocking)(struct uart_handle_t* handle, uint8_t byte);

    /** @brief Enables the transmit-related interrupts. */
    void (*enable_tx_interrupt)(struct uart_handle_t* handle);

    /** @brief Disables the transmit-related interrupts. */
    void (*disable_tx_interrupt)(struct uart_handle_t* handle);

    //... and so on for all required hardware interactions (RX, error handling, etc.)

} uart_port_interface_t;


#endif /* DRIVER_UART_PORT_UART_PORT_H_ */
