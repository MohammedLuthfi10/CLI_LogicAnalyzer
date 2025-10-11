/**
 * @file      iwdg_private.h
 * @brief     Private internal definitions for the IWDG driver.
 */

#ifndef IWDG_PRIVATE_H
#define IWDG_PRIVATE_H

#include "iwdg.h"
#include "port/iwdg_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct iwdg_handle_t {
    const iwdg_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // IWDG_PRIVATE_H
