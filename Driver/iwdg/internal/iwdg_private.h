/**
 * @file      uart_private.h
 * @brief     Private internal definitions for the UART driver.
 * @note      This file should NOT be included by application code.
 */

#ifndef UART_PRIVATE_H
#define UART_PRIVATE_H

#include "uart.h"
#include "uart_config.h"
#include "port/uart_port.h"

/**
 * @brief Internal runtime state for a UART instance.
 */
typedef struct {
    bool is_initialized;
    // Add other runtime state here (e.g., buffer pointers, error flags)
} uart_context_t;

/**
 * @brief The complete driver handle structure.
 * @note The full definition is private to the driver. The user only sees an
 *       opaque pointer (uart_handle_t).
 */
struct uart_handle_t {
    const uart_config_t config;           // User-provided configuration (read-only)
    uart_context_t context;               // Mutable runtime state
    const uart_port_interface_t* port_api; // Pointer to hardware porting functions
    void* port_hw_instance;               // Pointer to peripheral registers (e.g., USART1)
};

#endif // UART_PRIVATE_H
