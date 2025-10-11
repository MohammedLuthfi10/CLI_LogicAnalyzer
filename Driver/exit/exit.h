/**
 * @file      exti.h
 * @brief     Public API for the portable External Interrupt (EXTI) driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for configuring
 *            GPIO pins as external interrupt sources.
 */

#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing a configured EXTI line.
 */
typedef struct exti_handle_t* exti_handle_t;

/* --- Public Configuration Types --- */

/** @brief Defines the trigger condition for the external interrupt. */
typedef enum {
    EXTI_TRIGGER_RISING,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH,
} exti_trigger_t;

/**
 * @brief Callback function pointer type for EXTI events.
 * @param line_num The EXTI line number (0-15) that triggered the interrupt.
 * @param user_data A pointer to user-defined context, passed during init.
 */
typedef void (*exti_callback_t)(uint8_t line_num, void* user_data);

/**
 * @brief Configuration structure for EXTI line initialization.
 */
typedef struct {
    exti_trigger_t trigger;
    exti_callback_t callback;
    void* user_data;
} exti_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes and configures an EXTI line for a specific GPIO pin.
 *
 * @param[in] port_num The hardware port number (e.g., 0 for GPIOA, 1 for GPIOB).
 * @param[in] pin_num The pin number (0-15) to configure as an interrupt source.
 * @param[in] config Pointer to the configuration structure for the interrupt.
 *
 * @return A handle to the configured EXTI line, or NULL on failure.
 */
exti_handle_t exti_init(uint8_t port_num, uint8_t pin_num, const exti_config_t* config);

/**
 * @brief De-initializes an EXTI line.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void exti_deinit(exti_handle_t* p_handle);

#endif // EXTI_H
