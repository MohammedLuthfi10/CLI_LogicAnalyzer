/**
 * @file      iwdg.h
 * @brief     Public API for the portable Independent Watchdog (IWDG) driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for configuring and
 *            managing the hardware watchdog timer.
 */

#ifndef IWDG_H
#define IWDG_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing the IWDG instance.
 */
typedef struct iwdg_handle_t* iwdg_handle_t;

/* --- Public Configuration Types --- */

/**
 * @brief Configuration structure for IWDG initialization.
 */
typedef struct {
    uint32_t timeout_ms; // The desired timeout period in milliseconds.
} iwdg_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes and starts the Independent Watchdog timer.
 * @warning Once started, the IWDG cannot be stopped by software. The system
 *          must periodically call iwdg_feed() to prevent a reset.
 *
 * @param[in] config Pointer to the configuration structure for the watchdog.
 *
 * @return A handle to the IWDG instance, or NULL on failure.
 */
iwdg_handle_t iwdg_init(const iwdg_config_t* config);

/**
 * @brief De-initializes the IWDG handle.
 * @note This does NOT stop the watchdog timer, as it cannot be stopped by software.
 *       It only invalidates the driver handle.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void iwdg_deinit(iwdg_handle_t* p_handle);

/**
 * @brief Resets (feeds) the watchdog timer.
 * @details This function must be called periodically, before the configured
 *          timeout period elapses, to prevent a system reset.
 *
 * @param[in] handle The handle to the IWDG instance.
 */
void iwdg_feed(iwdg_handle_t handle);

#endif // IWDG_H
