/**
 * @file      adc_private.h
 * @brief     Private internal definitions for the ADC driver.
 */

#ifndef ADC_PRIVATE_H
#define ADC_PRIVATE_H

#include "adc.h"
#include "adc_config.h"
#include "port/adc_port.h"

/**
 * @brief Internal runtime state for an ADC instance.
 */
typedef struct {
    bool is_initialized;
} adc_context_t;

/**
 * @brief The complete driver handle structure.
 */
struct adc_handle_t {
    const adc_config_t config;
    adc_context_t context;
    const adc_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // ADC_PRIVATE_H
