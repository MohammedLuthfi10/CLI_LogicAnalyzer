/**
 * @file      uart_port.h
 * @brief     Defines the abstract porting interface for the UART driver.
 */

#ifndef UART_PORT_H
#define UART_PORT_H

// Forward declare the handle struct to avoid circular dependencies.
struct uart_handle_t;

/**
 * @brief A structure of function pointers that defines the hardware-dependent
 *        operations required by the UART driver.
 */
typedef struct {
    void (*enable_clock)(struct uart_handle_t* handle);
    void (*disable_clock)(struct uart_handle_t* handle);
    void (*init_pins)(struct uart_handle_t* handle);
    void (*configure_core)(struct uart_handle_t* handle);
    void (*write_byte_blocking)(struct uart_handle_t* handle, uint8_t byte);
    uint8_t (*read_byte_blocking)(struct uart_handle_t* handle);
    void (*enable)(struct uart_handle_t* handle);
    void (*disable)(struct uart_handle_t* handle);
} uart_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

/**
 * @brief Gets the porting API function table for a given instance.
 * @param instance_num The hardware instance number.
 * @return A pointer to the constant port interface struct, or NULL if invalid.
 */
const uart_port_interface_t* uart_port_get_api_for_instance(uint8_t instance_num);

/**
 * @brief Gets the peripheral base address for a given instance.
 * @param instance_num The hardware instance number.
 * @return A void pointer to the peripheral's base address, or NULL if invalid.
 */
void* uart_port_get_base_addr_for_instance(uint8_t instance_num);

/**
 * @brief Gets the peripheral clock frequency for a given instance.
 * @param instance_num The hardware instance number.
 * @return The clock frequency in Hz.
 */
uint32_t uart_port_get_clock_freq(uint8_t instance_num);

#endif // UART_PORT_H
