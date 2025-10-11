/**
 * @file      iwdg_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/iwdg_private.h"
#include "internal/iwdg_reg.h"

// Placeholder base address
#define IWDG_BASE             0x40003000UL

// --- Private Defines for STM32F4 ---
#define LSI_FREQUENCY_HZ 32000
#define MAX_RELOAD_VAL   0xFFF

// --- Port Implementation ---

static void stm32f4_start_and_configure(struct iwdg_handle_t* handle, uint32_t timeout_ms) {
    iwdg_reg_map_t* iwdg_regs = (iwdg_reg_map_t*)handle->port_hw_instance;

    // 1. Start the watchdog first
    iwdg_regs->KR = IWDG_KR_START;

    // 2. Unlock access to PR and RLR registers
    iwdg_regs->KR = IWDG_KR_UNLOCK;

    // 3. Calculate prescaler and reload values
    uint8_t prescaler_div = 4;
    uint8_t prescaler_reg = 0;

    // Find the smallest prescaler that allows the timeout to fit in the 12-bit reload register
    for (prescaler_reg = 0; prescaler_reg <= 6; ++prescaler_reg) {
        uint32_t reload_val = (timeout_ms * LSI_FREQUENCY_HZ) / (prescaler_div * 1000);
        if (reload_val <= MAX_RELOAD_VAL) {
            iwdg_regs->RLR = reload_val;
            break;
        }
        prescaler_div *= 2;
    }

    // If timeout is too large, clamp to max possible value
    if (prescaler_reg > 6) {
        prescaler_reg = 6; // DIV256
        iwdg_regs->RLR = MAX_RELOAD_VAL;
    }

    iwdg_regs->PR = prescaler_reg;

    // 4. Reload the counter to apply the new settings
    iwdg_regs->KR = IWDG_KR_RESET;
}

static void stm32f4_feed(struct iwdg_handle_t* handle) {
    ((iwdg_reg_map_t*)handle->port_hw_instance)->KR = IWDG_KR_RESET;
}

// --- The concrete port interface for STM32F4 ---
static const iwdg_port_interface_t stm32f4_port_api = {
 .start_and_configure = stm32f4_start_and_configure,
 .feed = stm32f4_feed,
};

// --- Public functions provided by the port ---
const iwdg_port_interface_t* iwdg_port_get_api(void) {
    return &stm32f4_port_api;
}

void* iwdg_port_get_base_addr(void) {
    return (void*)IWDG_BASE;
}
