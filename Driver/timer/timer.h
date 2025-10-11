/**
 * @file      timer.h
 * @brief     Public API for the portable Timer driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for configuring and
 *            controlling a general-purpose timer for periodic tasks.
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing a configured Timer instance.
 */
typedef struct timer_handle_t* timer_handle_t;

/* --- Public Configuration Types --- */

/**
 * @brief Configuration structure for Timer initialization.
 * @details This configures the timer as a basic up-counting periodic timer.
 *          The timer frequency will be (peripheral_clock / (prescaler + 1)).
 *          The update event (interrupt) frequency will be (timer_frequency / (period + 1)).
 */
typedef struct {
    uint32_t prescaler; // 16-bit or 32-bit value (0x0000 to 0xFFFF/0xFFFFFFFF)
    uint32_t period;    // 16-bit or 32-bit auto-reload value
} timer_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes and configures a general-purpose timer.
 *
 * @param[in] instance_num The hardware instance number (e.g., 2 for TIM2).
 * @param[in] config Pointer to the configuration structure for the timer.
 *
 * @return A handle to the configured timer, or NULL on failure.
 */
timer_handle_t timer_init(uint8_t instance_num, const timer_config_t* config);

/**
 * @brief De-initializes a timer instance.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void timer_deinit(timer_handle_t* p_handle);

/**
 * @brief Starts the timer counter.
 * @param[in] handle The handle to the timer instance.
 */
void timer_start(timer_handle_t handle);

/**
 * @brief Stops the timer counter.
 * @param[in] handle The handle to the timer instance.
 */
void timer_stop(timer_handle_t handle);

/**
 * @brief Gets the current value of the timer's counter.
 * @param[in] handle The handle to the timer instance.
 * @return The current 16-bit or 32-bit counter value.
 */
uint32_t timer_get_counter(timer_handle_t handle);

/**
 * @brief Enables the update interrupt for the timer.
 * @details The update interrupt is typically generated on counter overflow.
 * @param[in] handle The handle to the timer instance.
 */
void timer_enable_update_interrupt(timer_handle_t handle);

/**
 * @brief Disables the update interrupt for the timer.
 * @param[in] handle The handle to the timer instance.
 */
void timer_disable_update_interrupt(timer_handle_t handle);

/**
 * @brief Clears the update interrupt flag.
 * @details This function should be called inside the timer's interrupt service
 *          routine (ISR) to clear the hardware flag.
 * @param[in] handle The handle to the timer instance.
 * @return true if the update flag was set before clearing, false otherwise.
 */
bool timer_is_update_interrupt_flag_set(timer_handle_t handle);

/**
 * @brief Clears the update interrupt flag.
 * @param[in] handle The handle to the timer instance.
 */
void timer_clear_update_interrupt_flag(timer_handle_t handle);

#endif // TIMER_H
