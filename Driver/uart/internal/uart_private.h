/*
 * uart_private.h
 *
 *  Created on: Aug 6, 2025
 *      Author: hp
 */

#ifndef DRIVER_UART_INTERNAL_UART_PRIVATE_H_
#define DRIVER_UART_INTERNAL_UART_PRIVATE_H_


/**
 * @brief The complete driver handle structure.
 * @note The full definition is private to the driver. The user only sees an
 *       opaque pointer (uart_handle_t).
 */
struct uart_handle_t {
    const uart_config_t config; // User-provided configuration (read-only after init)
    uart_context_t context;     // Mutable runtime state
    const uart_port_interface_t* port_api; // Pointer to the hardware porting functions
    void* port_hw_instance;     // Pointer to the peripheral registers (e.g., UART1)
};



#endif /* DRIVER_UART_INTERNAL_UART_PRIVATE_H_ */
