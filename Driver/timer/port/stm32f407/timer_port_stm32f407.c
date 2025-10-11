/**
 * @file      timer_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/timer_private.h"
#include "internal/timer_reg.h"

// Placeholder base addresses
#define APB1PERIPH_BASE       0x40000000UL
#define APB2PERIPH_BASE       0x40010000UL
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00UL)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000UL)

// --- Private function implementations for STM32F4 ---

static void stm32f4_enable_clock(uint8_t instance_num) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., rcc_enable_peripheral_clock(PERIPH_ID_TIM2);
}

static void stm32f4_configure_core(struct timer_handle_t* handle) {
    timer_reg_map_t* timer_regs = (timer_reg_map_t*)handle->port_hw_instance;
    const timer_config_t* config = &handle->config;

    // Set basic up-counting, edge-aligned mode with auto-reload preload enabled
    timer_regs->CR1 = TIM_CR1_ARPE_Msk;
    timer_regs->PSC = config->prescaler;
    timer_regs->ARR = config->period;
}

static void stm32f4_start(struct timer_handle_t* handle) {
    ((timer_reg_map_t*)handle->port_hw_instance)->CR1 |= TIM_CR1_CEN_Msk;
}

static void stm32f4_stop(struct timer_handle_t* handle) {
    ((timer_reg_map_t*)handle->port_hw_instance)->CR1 &= ~TIM_CR1_CEN_Msk;
}

static uint32_t stm32f4_get_counter(struct timer_handle_t* handle) {
    return ((timer_reg_map_t*)handle->port_hw_instance)->CNT;
}

static void stm32f4_enable_update_irq(struct timer_handle_t* handle) {
    ((timer_reg_map_t*)handle->port_hw_instance)->DIER |= TIM_DIER_UIE_Msk;
}

static void stm32f4_disable_update_irq(struct timer_handle_t* handle) {
    ((timer_reg_map_t*)handle->port_hw_instance)->DIER &= ~TIM_DIER_UIE_Msk;
}

static bool stm32f4_is_update_irq_flag_set(struct timer_handle_t* handle) {
    return (((timer_reg_map_t*)handle->port_hw_instance)->SR & TIM_SR_UIF_Msk)!= 0;
}

static void stm32f4_clear_update_irq_flag(struct timer_handle_t* handle) {
    ((timer_reg_map_t*)handle->port_hw_instance)->SR &= ~TIM_SR_UIF_Msk;
}

// --- The concrete port interface for STM32F4 ---
static const timer_port_interface_t stm32f4_port_api = {
   .enable_clock = stm32f4_enable_clock,
   .configure_core = stm32f4_configure_core,
   .start = stm32f4_start,
   .stop = stm32f4_stop,
   .get_counter = stm32f4_get_counter,
   .enable_update_irq = stm32f4_enable_update_irq,
   .disable_update_irq = stm32f4_disable_update_irq,
   .is_update_irq_flag_set = stm32f4_is_update_irq_flag_set,
   .clear_update_irq_flag = stm32f4_clear_update_irq_flag,
};

// --- Public functions provided by the port ---
const timer_port_interface_t* timer_port_get_api(void) {
    return &stm32f4_port_api;
}

void* timer_port_get_base_addr(uint8_t instance_num) {
    switch (instance_num) {
        case 1: return (void*)TIM1_BASE;
        case 2: return (void*)TIM2_BASE;
        case 3: return (void*)TIM3_BASE;
        case 4: return (void*)TIM4_BASE;
        case 5: return (void*)TIM5_BASE;
        //... add other timers as needed
        default: return NULL;
    }
}
