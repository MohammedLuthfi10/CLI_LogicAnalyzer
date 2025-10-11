/**
 * @file      flash_private.h
 * @brief     Private internal definitions for the FLASH driver.
 */

#ifndef FLASH_PRIVATE_H
#define FLASH_PRIVATE_H

#include "flash.h"
#include "port/flash_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct flash_handle_t {
    const flash_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // FLASH_PRIVATE_H
