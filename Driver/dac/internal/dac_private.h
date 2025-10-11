/**
 * @file      dac_private.h
 * @brief     Private internal definitions for the DAC driver.
 */

#ifndef DAC_PRIVATE_H
#define DAC_PRIVATE_H

#include "dac.h"
#include "dac_config.h"
#include "port/dac_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct dac_handle_t {
    const dac_config_t config;
    const uint8_t channel_num;
    const dac_port_interface_t* port_api;
    void* port_hw_instance; // Pointer to the DAC controller (e.g., DAC1)
};

#endif // DAC_PRIVATE_H
