/**
 * @file      dac.h
 * @brief     Public API for the portable DAC driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for configuring and
 *            controlling a Digital-to-Analog Converter channel.
 */

#ifndef DAC_H
#define DAC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing a configured DAC channel.
 */
typedef struct dac_handle_t* dac_handle_t;

/* --- Public Configuration Types --- */

/**
 * @brief Configuration structure for DAC channel initialization.
 */
typedef struct {
    bool buffer_enabled; // Enable the output buffer for stronger drive capability.
} dac_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes and configures a DAC channel.
 *
 * @param[in] instance_num The hardware instance number (e.g., 1 for DAC1).
 * @param[in] channel_num The channel number to configure (e.g., 1 for Channel 1).
 * @param[in] config Pointer to the configuration structure for the channel.
 *
 * @return A handle to the configured DAC channel, or NULL on failure.
 */
dac_handle_t dac_init(uint8_t instance_num, uint8_t channel_num, const dac_config_t* config);

/**
 * @brief De-initializes a DAC channel.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void dac_deinit(dac_handle_t* p_handle);

/**
 * @brief Writes a 12-bit value to the DAC output.
 * @details This function loads the data into the DAC's data holding register
 *          and triggers the conversion.
 *
 * @param[in] handle The handle to the DAC channel.
 * @param[in] value The 12-bit value (0-4095) to be converted.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int dac_write_value(dac_handle_t handle, uint16_t value);

#endif // DAC_H
