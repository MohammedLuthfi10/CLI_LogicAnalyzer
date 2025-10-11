/**
 * @file      pwr_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/pwr_private.h"
#include "internal/pwr_reg.h"

// Placeholder base addresses
#define APB1PERIPH_BASE       0x40000000UL
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000UL)
#define SCB_BASE              0xE000ED00UL

#define SCB                   ((scb_reg_map_t*) SCB_BASE)

// --- Port Implementation ---

static void stm32f4_enable_clock(void) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., rcc_enable_peripheral_clock(PERIPH_ID_PWR);
}

static void stm32f4_set_voltage_scale(struct pwr_handle_t* handle, pwr_voltage_scale_t scale) {
    pwr_reg_map_t* pwr_regs = (pwr_reg_map_t*)handle->port_hw_instance;
    uint32_t vos_val = 0;

    // Mapping from abstract scale to STM32F4-specific values
    // Note: The STM32F4xx reference manual specifies VOS[1:0] bits.
    // 0b11 = Scale 1, 0b10 = Scale 2, 0b01 = Scale 3.
    switch (scale) {
        case PWR_VOLTAGE_SCALE_1: vos_val = 3; break;
        case PWR_VOLTAGE_SCALE_2: vos_val = 2; break;
        case PWR_VOLTAGE_SCALE_3: vos_val = 1; break;
    }

    pwr_regs->CR &= ~PWR_CR_VOS_Msk;
    pwr_regs->CR |= (vos_val << PWR_CR_VOS_Pos);
}

static void stm32f4_enter_stop_mode(struct pwr_handle_t* handle) {
    pwr_reg_map_t* pwr_regs = (pwr_reg_map_t*)handle->port_hw_instance;

    // Clear PDDS bit to select STOP mode on deepsleep
    pwr_regs->CR &= ~PWR_CR_PDDS_Msk;
    // Set SLEEPDEEP bit in Cortex-M System Control Register
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Wait for interrupt
    __asm__("WFI");

    // After wakeup, clear SLEEPDEEP bit
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
}

static void stm32f4_enter_standby_mode(struct pwr_handle_t* handle) {
    pwr_reg_map_t* pwr_regs = (pwr_reg_map_t*)handle->port_hw_instance;

    // Clear Wakeup flag
    pwr_regs->CR |= PWR_CR_CSBF_Msk;
    // Set PDDS bit to select STANDBY mode on deepsleep
    pwr_regs->CR |= PWR_CR_PDDS_Msk;
    // Set SLEEPDEEP bit in Cortex-M System Control Register
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Wait for interrupt (which will trigger standby and not return)
    __asm__("WFI");
}

static void stm32f4_disable_backup_domain_write_protect(struct pwr_handle_t* handle) {
    ((pwr_reg_map_t*)handle->port_hw_instance)->CR |= PWR_CR_DBP_Msk;
}

static void stm32f4_enable_backup_domain_write_protect(struct pwr_handle_t* handle) {
    ((pwr_reg_map_t*)handle->port_hw_instance)->CR &= ~PWR_CR_DBP_Msk;
}

// --- The concrete port interface for STM32F4 ---
static const pwr_port_interface_t stm32f4_port_api = {
.enable_clock = stm32f4_enable_clock,
.set_voltage_scale = stm32f4_set_voltage_scale,
.enter_stop_mode = stm32f4_enter_stop_mode,
.enter_standby_mode = stm32f4_enter_standby_mode,
.disable_backup_domain_write_protect = stm32f4_disable_backup_domain_write_protect,
.enable_backup_domain_write_protect = stm32f4_enable_backup_domain_write_protect,
};

// --- Public functions provided by the port ---
const pwr_port_interface_t* pwr_port_get_api(void) {
    return &stm32f4_port_api;
}

void* pwr_port_get_base_addr(void) {
    return (void*)PWR_BASE;
}
