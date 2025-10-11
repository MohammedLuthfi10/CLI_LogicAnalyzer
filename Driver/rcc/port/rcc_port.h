/**
 * @file      rcc_port.h
 * @brief     Defines the abstract porting interface for the RCC driver.
 */

#ifndef RCC_PORT_H
#define RCC_PORT_H

#include "rcc.h"

/** @brief Structure to hold calculated bus frequencies. */
typedef struct {
    uint32_t ahb_frequency;
    uint32_t apb1_frequency;
    uint32_t apb2_frequency;
} rcc_bus_frequencies_t;

/* --- Functions to be provided by the concrete port implementation --- */

/**
 * @brief Platform-specific function to configure the entire clock tree.
 *
 * @param[in] clk_source The primary clock source.
 * @param[in] external_crystal_hz Frequency of the external crystal.
 * @param[in] sysclk_freq The target system clock frequency.
 * @param[out] out_frequencies Pointer to a struct to store the resulting bus frequencies.
 *
 * @return true on success, false on failure.
 */
bool rcc_port_system_init(rcc_clock_source_t clk_source, uint32_t external_crystal_hz, rcc_sysclk_freq_t sysclk_freq, rcc_bus_frequencies_t* out_frequencies);

/**
 * @brief Platform-specific function to enable a peripheral's clock.
 * @param[in] id The generic peripheral identifier.
 */
void rcc_port_enable_peripheral_clock(peripheral_id_t id);

/**
 * @brief Platform-specific function to disable a peripheral's clock.
 * @param[in] id The generic peripheral identifier.
 */
void rcc_port_disable_peripheral_clock(peripheral_id_t id);

#endif // RCC_PORT_H
