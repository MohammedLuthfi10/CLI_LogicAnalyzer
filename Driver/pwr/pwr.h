/**
 * @file      pwr.h
 * @brief     Public API for the portable Power Control (PWR) driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for managing the
 *            microcontroller's power modes and voltage scaling.
 */

#ifndef PWR_H
#define PWR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing the PWR controller instance.
 */
typedef struct pwr_handle_t* pwr_handle_t;

/* --- Public Configuration Types --- */

/** @brief Main regulator voltage scaling for performance/power trade-off. */
typedef enum {
    PWR_VOLTAGE_SCALE_1, // High performance
    PWR_VOLTAGE_SCALE_2, // Medium performance
    PWR_VOLTAGE_SCALE_3, // Low power
} pwr_voltage_scale_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes the PWR controller interface.
 * @details This function should be called once to get a handle to the controller.
 *
 * @return A handle to the PWR controller, or NULL on failure.
 */
pwr_handle_t pwr_init(void);

/**
 * @brief De-initializes the PWR controller handle.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void pwr_deinit(pwr_handle_t* p_handle);

/**
 * @brief Sets the main regulator voltage scaling.
 * @details This is a critical function for balancing performance and power
 *          consumption, and is often required when changing system clock speeds.
 *
 * @param[in] handle The handle to the PWR controller.
 * @param[in] scale The desired voltage scale.
 */
void pwr_set_voltage_scale(pwr_handle_t handle, pwr_voltage_scale_t scale);

/**
 * @brief Enters STOP mode.
 * @details This is a low-power mode where the CPU clock is stopped. The MCU
 *          can be woken up by an EXTI interrupt. This function will block
 *          until the MCU is woken up.
 *
 * @param[in] handle The handle to the PWR controller.
 */
void pwr_enter_stop_mode(pwr_handle_t handle);

/**
 * @brief Enters STANDBY mode.
 * @details This is the lowest-power mode. The MCU can only be woken up by a
 *          wakeup pin, RTC alarm, or a system reset. Execution will restart
 *          from the beginning as if a reset occurred. This function does not return.
 *
 * @param[in] handle The handle to the PWR controller.
 */
void pwr_enter_standby_mode(pwr_handle_t handle);

/**
 * @brief Disables write protection for the backup domain.
 * @details This is required to configure the RTC and LSE oscillator.
 *
 * @param[in] handle The handle to the PWR controller.
 */
void pwr_disable_backup_domain_write_protect(pwr_handle_t handle);

/**
 * @brief Enables write protection for the backup domain.
 *
 * @param[in] handle The handle to the PWR controller.
 */
void pwr_enable_backup_domain_write_protect(pwr_handle_t handle);

#endif // PWR_H
