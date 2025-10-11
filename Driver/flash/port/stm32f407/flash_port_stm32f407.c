/**
 * @file      flash_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/flash_private.h"
#include "internal/flash_reg.h"

// Placeholder base address
#define FLASH_R_BASE          0x40023C00UL

// --- Private Helper Functions for STM32F4 ---

static void stm32f4_wait_for_last_operation(flash_reg_map_t* flash_regs) {
    while (flash_regs->SR & FLASH_SR_BSY_Msk);
}

static void stm32f4_unlock(flash_reg_map_t* flash_regs) {
    if (flash_regs->CR & FLASH_CR_LOCK_Msk) {
        flash_regs->KEYR = FLASH_KEYR_KEY1;
        flash_regs->KEYR = FLASH_KEYR_KEY2;
    }
}

static void stm32f4_lock(flash_reg_map_t* flash_regs) {
    flash_regs->CR |= FLASH_CR_LOCK_Msk;
}

// --- Port Implementation ---

static int stm32f4_set_wait_states(struct flash_handle_t* handle, uint32_t system_clock_hz) {
    flash_reg_map_t* flash_regs = (flash_reg_map_t*)handle->port_hw_instance;
    uint32_t ws = 0;

    // These values are examples for a specific STM32F4 variant at 3.3V.
    // Refer to the device datasheet for correct values.
    if (system_clock_hz <= 30000000) { ws = 0; }
    else if (system_clock_hz <= 60000000) { ws = 1; }
    else if (system_clock_hz <= 90000000) { ws = 2; }
    else if (system_clock_hz <= 120000000) { ws = 3; }
    else if (system_clock_hz <= 150000000) { ws = 4; }
    else if (system_clock_hz <= 180000000) { ws = 5; }
    else { return -1; /* Unsupported frequency */ }

    flash_regs->ACR &= ~FLASH_ACR_LATENCY_Msk;
    flash_regs->ACR |= (ws << FLASH_ACR_LATENCY_Pos);

    return 0;
}

static int stm32f4_erase_sector(struct flash_handle_t* handle, uint8_t sector_index) {
    flash_reg_map_t* flash_regs = (flash_reg_map_t*)handle->port_hw_instance;

    stm32f4_wait_for_last_operation(flash_regs);
    stm32f4_unlock(flash_regs);

    // Clear status flags
    flash_regs->SR = 0xFFFFFFFF;

    // Set sector erase and sector number
    flash_regs->CR &= ~(FLASH_CR_PSIZE_Msk | FLASH_CR_SNB_Msk);
    flash_regs->CR |= FLASH_CR_SER_Msk | (sector_index << FLASH_CR_SNB_Pos);

    // Start erase
    flash_regs->CR |= FLASH_CR_STRT_Msk;

    stm32f4_wait_for_last_operation(flash_regs);

    // Clear SER bit
    flash_regs->CR &= ~FLASH_CR_SER_Msk;

    stm32f4_lock(flash_regs);

    return 0;
}

static int stm32f4_program(struct flash_handle_t* handle, uint32_t address, const uint8_t* data, size_t len) {
    flash_reg_map_t* flash_regs = (flash_reg_map_t*)handle->port_hw_instance;

    stm32f4_wait_for_last_operation(flash_regs);
    stm32f4_unlock(flash_regs);

    // Clear status flags
    flash_regs->SR = 0xFFFFFFFF;

    // Set program size to byte and enable programming
    flash_regs->CR &= ~FLASH_CR_PSIZE_Msk; // PSIZE = x8
    flash_regs->CR |= FLASH_CR_PG_Msk;

    for (size_t i = 0; i < len; ++i) {
        *(volatile uint8_t*)(address + i) = data[i];
        stm32f4_wait_for_last_operation(flash_regs);
    }

    // Disable programming
    flash_regs->CR &= ~FLASH_CR_PG_Msk;

    stm32f4_lock(flash_regs);

    return 0;
}

// --- The concrete port interface for STM32F4 ---
static const flash_port_interface_t stm32f4_port_api = {
 .set_wait_states = stm32f4_set_wait_states,
 .erase_sector = stm32f4_erase_sector,
 .program = stm32f4_program,
};

// --- Public functions provided by the port ---
const flash_port_interface_t* flash_port_get_api(void) {
    return &stm32f4_port_api;
}

void* flash_port_get_base_addr(void) {
    return (void*)FLASH_R_BASE;
}
