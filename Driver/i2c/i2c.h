/**
 * @file      i2c.h
 * @brief     Public API for the portable I2C driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions and data structures
 *            for interacting with an I2C peripheral in master mode.
 */

#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing an I2C driver instance.
 */
typedef struct i2c_handle_t* i2c_handle_t;

/* --- Public Configuration Types --- */

/** @brief I2C bus speed configuration. */
typedef enum {
    I2C_SPEED_STANDARD_100KHZ = 0,
    I2C_SPEED_FAST_400KHZ,
} i2c_speed_t;

/**
 * @brief Configuration structure for I2C initialization (Master Mode).
 */
typedef struct {
    i2c_speed_t speed;
    uint32_t peripheral_clock_hz; // The clock frequency of the I2C peripheral itself (e.g., APB1 clock)
} i2c_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes an I2C peripheral instance in master mode.
 *
 * @param[in] instance_num The hardware instance number (e.g., 1 for I2C1).
 * @param[in] config Pointer to the user-provided configuration structure.
 *
 * @return A handle to the initialized I2C instance, or NULL on failure.
 */
i2c_handle_t i2c_init(uint8_t instance_num, const i2c_config_t* config);

/**
 * @brief De-initializes an I2C peripheral instance.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void i2c_deinit(i2c_handle_t* p_handle);

/**
 * @brief Writes a block of data to a 7-bit slave address in blocking mode.
 *
 * @param[in] handle The handle to the I2C instance.
 * @param[in] slave_addr The 7-bit address of the slave device.
 * @param[in] p_data Pointer to the data buffer to transmit.
 * @param[in] len The number of bytes to transmit.
 *
 * @return 0 on success, or a negative error code on failure (e.g., NACK).
 */
int i2c_master_write_blocking(i2c_handle_t handle, uint8_t slave_addr, const uint8_t* p_data, size_t len);

/**
 * @brief Reads a block of data from a 7-bit slave address in blocking mode.
 *
 * @param[in] handle The handle to the I2C instance.
 * @param[in] slave_addr The 7-bit address of the slave device.
 * @param[out] p_data Pointer to the buffer where received data will be stored.
 * @param[in] len The number of bytes to read.
 *
 * @return 0 on success, or a negative error code on failure (e.g., NACK).
 */
int i2c_master_read_blocking(i2c_handle_t handle, uint8_t slave_addr, uint8_t* p_data, size_t len);

#endif // I2C_H
