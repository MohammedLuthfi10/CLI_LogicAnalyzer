/**
 * @file      timer_private.h
 * @brief     Private internal definitions for the Timer driver.
 */

#ifndef TIMER_PRIVATE_H
#define TIMER_PRIVATE_H

#include "timer.h"
#include "timer_config.h"
#include "port/timer_port.h"

/**
 * @brief Internal runtime state for a Timer instance.
 */
typedef struct {
    bool is_initialized;
} timer_context_t;

/**
 * @brief The complete driver handle structure.
 */
struct timer_handle_t {
    const timer_config_t config;
    timer_context_t context;
    const timer_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // TIMER_PRIVATE_H
