/**
 * @file      rng_private.h
 * @brief     Private internal definitions for the RNG driver.
 */

#ifndef RNG_PRIVATE_H
#define RNG_PRIVATE_H

#include "rng.h"
#include "port/rng_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct rng_handle_t {
    const rng_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // RNG_PRIVATE_H
