/**
 * @file      rng_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/rng_private.h"
#include "internal/rng_reg.h"

// Placeholder base address
#define AHB2PERIPH_BASE       0x40020000UL
#define RNG_BASE              (AHB2PERIPH_BASE + 0x60800UL)

// --- Port Implementation ---

static void stm32f4_enable_clock(void) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., rcc_enable_peripheral_clock(PERIPH_ID_RNG);
}

static void stm32f4_enable(struct rng_handle_t* handle) {
    ((rng_reg_map_t*)handle->port_hw_instance)->CR |= RNG_CR_RNGEN_Msk;
}

static void stm32f4_disable(struct rng_handle_t* handle) {
    ((rng_reg_map_t*)handle->port_hw_instance)->CR &= ~RNG_CR_RNGEN_Msk;
}

static int stm32f4_get_random_blocking(struct rng_handle_t* handle, uint32_t* random_num) {
    rng_reg_map_t* rng_regs = (rng_reg_map_t*)handle->port_hw_instance;

    // Wait for data to be ready
    while (!(rng_regs->SR & RNG_SR_DRDY_Msk));

    // Check for errors
    if (rng_regs->SR & (RNG_SR_CECS_Msk | RNG_SR_SECS_Msk)) {
        return -1; // Hardware error
    }

    *random_num = rng_regs->DR;
    return 0;
}

// --- The concrete port interface for STM32F4 ---
static const rng_port_interface_t stm32f4_port_api = {
.enable_clock = stm32f4_enable_clock,
.enable = stm32f4_enable,
.disable = stm32f4_disable,
.get_random_blocking = stm32f4_get_random_blocking,
};

// --- Public functions provided by the port ---
const rng_port_interface_t* rng_port_get_api(void) {
    return &stm32f4_port_api;
}

void* rng_port_get_base_addr(void) {
    return (void*)RNG_BASE;
}
