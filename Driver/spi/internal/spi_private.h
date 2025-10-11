/**
 * @file      spi_private.h
 * @brief     Private internal definitions for the SPI driver.
 * @note      This file should NOT be included by application code.
 */

#ifndef SPI_PRIVATE_H
#define SPI_PRIVATE_H

#include "spi.h"
#include "spi_config.h"
#include "port/spi_port.h"

/**
 * @brief Internal runtime state for an SPI instance.
 */
typedef struct {
    bool is_initialized;
} spi_context_t;

/**
 * @brief The complete driver handle structure.
 */
struct spi_handle_t {
    const spi_config_t config;
    spi_context_t context;
    const spi_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // SPI_PRIVATE_H
