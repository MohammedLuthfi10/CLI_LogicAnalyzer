/**
 * @file      adc.h
 * @brief     Public API for the portable ADC driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for configuring and
 *            performing single-channel conversions with an ADC peripheral.
 */

#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing a configured ADC instance.
 */
typedef struct adc_handle_t* adc_handle_t;

/* --- Public Configuration Types --- */

/** @brief ADC conversion resolution. */
typedef enum {
    ADC_RESOLUTION_12_BIT = 0,
    ADC_RESOLUTION_10_BIT,
    ADC_RESOLUTION_8_BIT,
    ADC_RESOLUTION_6_BIT,
} adc_resolution_t;

/** @brief ADC channel sample time. */
typedef enum {
    ADC_SAMPLE_TIME_3_CYCLES = 0,
    ADC_SAMPLE_TIME_15_CYCLES,
    ADC_SAMPLE_TIME_28_CYCLES,
    ADC_SAMPLE_TIME_56_CYCLES,
    ADC_SAMPLE_TIME_84_CYCLES,
    ADC_SAMPLE_TIME_112_CYCLES,
    ADC_SAMPLE_TIME_144_CYCLES,
    ADC_SAMPLE_TIME_480_CYCLES,
} adc_sample_time_t;

/**
 * @brief Configuration structure for ADC initialization.
 */
typedef struct {
    adc_resolution_t resolution;
    adc_sample_time_t default_sample_time; // Sample time to use for all channels
} adc_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes and configures an ADC peripheral.
 *
 * @param[in] instance_num The hardware instance number (e.g., 1 for ADC1).
 * @param[in] config Pointer to the configuration structure for the ADC.
 *
 * @return A handle to the configured ADC, or NULL on failure.
 */
adc_handle_t adc_init(uint8_t instance_num, const adc_config_t* config);

/**
 * @brief De-initializes an ADC instance.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void adc_deinit(adc_handle_t* p_handle);

/**
 * @brief Performs a single conversion on a specified channel in blocking mode.
 *
 * @param[in] handle The handle to the ADC instance.
 * @param[in] channel The channel number to convert (0-18).
 *
 * @return The 16-bit conversion result, right-aligned. Returns 0 on error.
 */
uint16_t adc_read_blocking(adc_handle_t handle, uint8_t channel);

#endif // ADC_H
