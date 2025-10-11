/**
 * @file      pwr_private.h
 * @brief     Private internal definitions for the PWR driver.
 */

#ifndef PWR_PRIVATE_H
#define PWR_PRIVATE_H

#include "pwr.h"
#include "port/pwr_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct pwr_handle_t {
    const pwr_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // PWR_PRIVATE_H
