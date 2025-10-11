/**
 * @file      rng_port.h
 * @brief     Defines the abstract porting interface for the RNG driver.
 */

#ifndef RNG_PORT_H
#define RNG_PORT_H

#include "rng.h"

struct rng_handle_t;

typedef struct {
    void (*enable_clock)(void);
    void (*enable)(struct rng_handle_t* handle);
    void (*disable)(struct rng_handle_t* handle);
    int (*get_random_blocking)(struct rng_handle_t* handle, uint32_t* random_num);
} rng_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const rng_port_interface_t* rng_port_get_api(void);
void* rng_port_get_base_addr(void);

#endif // RNG_PORT_H
