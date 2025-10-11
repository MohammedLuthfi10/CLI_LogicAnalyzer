/**
 * @file      i2c_private.h
 * @brief     Private internal definitions for the I2C driver.
 */

#ifndef I2C_PRIVATE_H
#define I2C_PRIVATE_H

#include "i2c.h"
#include "i2c_config.h"
#include "port/i2c_port.h"

/**
 * @brief Internal runtime state for an I2C instance.
 */
typedef struct {
    bool is_initialized;
} i2c_context_t;

/**
 * @brief The complete driver handle structure.
 */
struct i2c_handle_t {
    const i2c_config_t config;
    i2c_context_t context;
    const i2c_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // I2C_PRIVATE_H
