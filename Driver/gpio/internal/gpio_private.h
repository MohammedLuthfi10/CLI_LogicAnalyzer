/**
 * @file      gpio_private.h
 * @brief     Private internal definitions for the GPIO driver.
 */

#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include "gpio.h"
#include "gpio_config.h"
#include "port/gpio_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct gpio_handle_t {
    const gpio_config_t config;
    const uint16_t pin_mask;              // Bitmask of pins this handle controls
    const const void* port_hw_instance;   // Pointer to peripheral registers (e.g., GPIOA)
    const gpio_port_interface_t* port_api; // Pointer to hardware porting functions
};

#endif // GPIO_PRIVATE_H
