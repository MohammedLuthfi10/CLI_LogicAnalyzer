/**
 * @file      rcc.c
 * @brief     Hardware-agnostic implementation of the RCC driver.
 */

#include "internal/rcc_private.h"

// --- Static Data ---
static rcc_context_t s_rcc_context = {.is_initialized = false };

// --- Public API Function Implementations ---

bool rcc_system_init(rcc_clock_source_t clk_source, uint32_t external_crystal_hz, rcc_sysclk_freq_t sysclk_freq) {
    rcc_bus_frequencies_t freqs;
    bool success = rcc_port_system_init(clk_source, external_crystal_hz, sysclk_freq, &freqs);

    if (success) {
        s_rcc_context.ahb_frequency = freqs.ahb_frequency;
        s_rcc_context.apb1_frequency = freqs.apb1_frequency;
        s_rcc_context.apb2_frequency = freqs.apb2_frequency;
        s_rcc_context.is_initialized = true;
    }

    return success;
}

void rcc_enable_peripheral_clock(peripheral_id_t id) {
    rcc_port_enable_peripheral_clock(id);
}

void rcc_disable_peripheral_clock(peripheral_id_t id) {
    rcc_port_disable_peripheral_clock(id);
}

uint32_t rcc_get_ahb_frequency(void) {
    return s_rcc_context.is_initialized? s_rcc_context.ahb_frequency : 0;
}

uint32_t rcc_get_apb1_frequency(void) {
    return s_rcc_context.is_initialized? s_rcc_context.apb1_frequency : 0;
}

uint32_t rcc_get_apb2_frequency(void) {
    return s_rcc_context.is_initialized? s_rcc_context.apb2_frequency : 0;
}
