/**
 * @file      dac_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/dac_private.h"
#include "internal/dac_reg.h"

// Placeholder base address
#define APB1PERIPH_BASE       0x40000000UL
#define DAC_BASE              (APB1PERIPH_BASE + 0x7400UL)

// --- Private function implementations for STM32F4 ---

static void stm32f4_enable_clock(uint8_t instance_num) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., rcc_enable_peripheral_clock(PERIPH_ID_DAC);
}

static void stm32f4_init_pins(uint8_t channel_num) {
    // Placeholder: A real implementation would use a GPIO driver to
    // configure PA4 (for DAC_OUT1) or PA5 (for DAC_OUT2) as analog.
}

static void stm32f4_configure_channel(struct dac_handle_t* handle) {
    dac_reg_map_t* dac_regs = (dac_reg_map_t*)handle->port_hw_instance;
    const dac_config_t* config = &handle->config;

    if (handle->channel_num == 1) {
        // Enable software trigger
        dac_regs->CR |= (7U << DAC_CR_TSEL1_Pos) | DAC_CR_TEN1_Msk;
        // Set buffer state
        if (config->buffer_enabled) {
            dac_regs->CR &= ~DAC_CR_BOFF1_Msk;
        } else {
            dac_regs->CR |= DAC_CR_BOFF1_Msk;
        }
        // Enable channel
        dac_regs->CR |= DAC_CR_EN1_Msk;
    } else if (handle->channel_num == 2) {
        // Enable software trigger
        dac_regs->CR |= (7U << DAC_CR_TSEL2_Pos) | DAC_CR_TEN2_Msk;
        // Set buffer state
        if (config->buffer_enabled) {
            dac_regs->CR &= ~DAC_CR_BOFF2_Msk;
        } else {
            dac_regs->CR |= DAC_CR_BOFF2_Msk;
        }
        // Enable channel
        dac_regs->CR |= DAC_CR_EN2_Msk;
    }
}

static void stm32f4_write_value(struct dac_handle_t* handle, uint16_t value) {
    dac_reg_map_t* dac_regs = (dac_reg_map_t*)handle->port_hw_instance;

    if (handle->channel_num == 1) {
        dac_regs->DHR12R1 = value & 0xFFF;
        dac_regs->SWTRIGR |= DAC_SWTRIGR_SWTRIG1_Msk;
    } else if (handle->channel_num == 2) {
        dac_regs->DHR12R2 = value & 0xFFF;
        dac_regs->SWTRIGR |= DAC_SWTRIGR_SWTRIG2_Msk;
    }
}

// --- The concrete port interface for STM32F4 ---
static const dac_port_interface_t stm32f4_port_api = {
  .enable_clock = stm32f4_enable_clock,
  .init_pins = stm32f4_init_pins,
  .configure_channel = stm32f4_configure_channel,
  .write_value = stm32f4_write_value,
};

// --- Public functions provided by the port ---
const dac_port_interface_t* dac_port_get_api(void) {
    return &stm32f4_port_api;
}

void* dac_port_get_base_addr(uint8_t instance_num) {
    if (instance_num == 1) {
        return (void*)DAC_BASE;
    }
    return NULL;
}
