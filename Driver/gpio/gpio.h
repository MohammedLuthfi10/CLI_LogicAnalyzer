/**
 * @file      gpio.h
 * @brief     Public API for the portable GPIO driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions and data structures
 *            for configuring and controlling General-Purpose I/O pins.
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing a configured GPIO pin or group of pins.
 */
typedef struct gpio_handle_t* gpio_handle_t;

/* --- Public Configuration Types --- */

/** @brief GPIO pin mode. */
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE_FUNCTION,
    GPIO_MODE_ANALOG,
} gpio_mode_t;

/** @brief GPIO internal pull-up / pull-down resistor configuration. */
typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
} gpio_pull_t;

/** @brief GPIO output driver type. */
typedef enum {
    GPIO_OUTPUT_TYPE_PUSH_PULL = 0,
    GPIO_OUTPUT_TYPE_OPEN_DRAIN,
} gpio_output_type_t;

/** @brief GPIO output speed. */
typedef enum {
    GPIO_SPEED_LOW = 0,     // Corresponds to 2MHz on STM32F4
    GPIO_SPEED_MEDIUM,  // Corresponds to 25MHz on STM32F4
    GPIO_SPEED_HIGH,    // Corresponds to 50MHz on STM32F4
    GPIO_SPEED_VERY_HIGH, // Corresponds to 100MHz on STM32F4
} gpio_speed_t;

/**
 * @brief Configuration structure for GPIO pin initialization.
 */
typedef struct {
    gpio_mode_t mode;
    gpio_pull_t pull;
    // The following settings only apply for OUTPUT or AF modes
    gpio_output_type_t output_type;
    gpio_speed_t speed;
    uint8_t alternate_function; // AF number (0-15), only for AF mode
} gpio_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes and configures a GPIO pin or group of pins.
 *
 * @param[in] port_num The hardware port number (e.g., 0 for GPIOA, 1 for GPIOB).
 * @param[in] pin_mask A bitmask of the pins to configure (e.g., (1 << 5) for pin 5).
 * @param[in] config Pointer to the configuration structure for the pins.
 *
 * @return A handle to the configured GPIO pins, or NULL on failure.
 */
gpio_handle_t gpio_init(uint8_t port_num, uint16_t pin_mask, const gpio_config_t* config);

/**
 * @brief De-initializes GPIO pins associated with a handle.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void gpio_deinit(gpio_handle_t* p_handle);

/**
 * @brief Sets the output of the configured GPIO pin(s) to high.
 * @param[in] handle The handle to the GPIO pin(s).
 */
void gpio_set(gpio_handle_t handle);

/**
 * @brief Clears the output of the configured GPIO pin(s) to low.
 * @param[in] handle The handle to the GPIO pin(s).
 */
void gpio_clear(gpio_handle_t handle);

/**
 * @brief Toggles the output of the configured GPIO pin(s).
 * @param[in] handle The handle to the GPIO pin(s).
 */
void gpio_toggle(gpio_handle_t handle);

/**
 * @brief Reads the input level of the configured GPIO pin.
 * @note If the handle manages multiple pins, this reads the state of the
 *       lowest-numbered pin in the group.
 * @param[in] handle The handle to the GPIO pin.
 * @return true if the pin is high, false if it is low.
 */
bool gpio_read(gpio_handle_t handle);

#endif // GPIO_H
