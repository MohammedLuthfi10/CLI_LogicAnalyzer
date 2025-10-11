/**
 * @file      rcc_private.h
 * @brief     Private internal definitions for the RCC driver.
 */

#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include "rcc.h"
#include "port/rcc_port.h"

/**
 * @brief Internal state for the RCC driver.
 * @details Stores the calculated bus frequencies after initialization.
 */
typedef struct {
    bool is_initialized;
    uint32_t ahb_frequency;
    uint32_t apb1_frequency;
    uint32_t apb2_frequency;
} rcc_context_t;

#endif // RCC_PRIVATE_H
