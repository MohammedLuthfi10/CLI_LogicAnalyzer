/**
 * @file      exti_private.h
 * @brief     Private internal definitions for the EXTI driver.
 */

#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

#include "exti.h"
#include "exti_config.h"
#include "port/exti_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct exti_handle_t {
    const exti_config_t config;
    const uint8_t pin_num;
    const exti_port_interface_t* port_api;
};

#endif // EXTI_PRIVATE_H
