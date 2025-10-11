/**
 * @file      exti_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/exti_private.h"
#include "internal/exti_reg.h"

// Placeholder base addresses
#define APB2PERIPH_BASE       0x40010000UL
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00UL)
#define NVIC_ISER_BASE        0xE000E100UL // Interrupt Set-Enable Registers

#define SYSCFG                ((syscfg_reg_map_t*) SYSCFG_BASE)
#define EXTI                  ((exti_reg_map_t*) EXTI_BASE)
#define NVIC_ISER(n)          (((volatile uint32_t*) NVIC_ISER_BASE)[n])

// --- Private Data ---
static exti_generic_handler_t s_generic_handler = NULL;

// --- Port Implementation ---

static void stm32f4_enable_syscfg_clock(void) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., rcc_enable_peripheral_clock(PERIPH_ID_SYSCFG);
}

static void stm32f4_configure_interrupt(uint8_t port_num, uint8_t pin_num, exti_trigger_t trigger) {
    // 1. Select GPIO port for the EXTI line
    uint8_t reg_index = pin_num / 4;
    uint8_t shift = (pin_num % 4) * 4;
    SYSCFG->EXTICR[reg_index] &= ~(0xF << shift);
    SYSCFG->EXTICR[reg_index] |= (port_num << shift);

    // 2. Configure trigger type
    if (trigger == EXTI_TRIGGER_RISING |

| trigger == EXTI_TRIGGER_BOTH) {
        EXTI->RTSR |= (1 << pin_num);
    } else {
        EXTI->RTSR &= ~(1 << pin_num);
    }
    if (trigger == EXTI_TRIGGER_FALLING |

| trigger == EXTI_TRIGGER_BOTH) {
        EXTI->FTSR |= (1 << pin_num);
    } else {
        EXTI->FTSR &= ~(1 << pin_num);
    }
}

static void stm32f4_enable_irq(uint8_t pin_num, exti_generic_handler_t handler) {
    s_generic_handler = handler;

    // Enable interrupt in EXTI peripheral
    EXTI->IMR |= (1 << pin_num);

    // Enable interrupt in NVIC
    if (pin_num <= 4) {
        NVIC_ISER(0) |= (1 << (pin_num + 6)); // IRQ numbers 6-10 for EXTI0-4
    } else if (pin_num <= 9) {
        NVIC_ISER(0) |= (1 << 23); // IRQ number 23 for EXTI9_5
    } else {
        NVIC_ISER(1) |= (1 << 8);  // IRQ number 40 for EXTI15_10
    }
}

static void stm32f4_disable_irq(uint8_t pin_num) {
    EXTI->IMR &= ~(1 << pin_num);
    // Disabling in NVIC is more complex as IRQs are shared.
    // For simplicity, we only disable the EXTI mask here.
}

// --- The concrete port interface for STM32F4 ---
static const exti_port_interface_t stm32f4_port_api = {
  .enable_syscfg_clock = stm32f4_enable_syscfg_clock,
  .configure_interrupt = stm32f4_configure_interrupt,
  .enable_irq = stm32f4_enable_irq,
  .disable_irq = stm32f4_disable_irq,
};

const exti_port_interface_t* exti_port_get_api(void) {
    return &stm32f4_port_api;
}

// --- ISR Handlers ---
// These are the actual hardware ISRs. They check which line triggered the
// interrupt, clear the pending flag, and call the generic handler.

void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1 << 0)) {
        EXTI->PR = (1 << 0); // Clear pending bit
        if (s_generic_handler) s_generic_handler(0);
    }
}

void EXTI1_IRQHandler(void) {
    if (EXTI->PR & (1 << 1)) {
        EXTI->PR = (1 << 1);
        if (s_generic_handler) s_generic_handler(1);
    }
}
//... Add handlers for EXTI2, EXTI3, EXTI4...

void EXTI9_5_IRQHandler(void) {
    for (uint8_t i = 5; i <= 9; ++i) {
        if (EXTI->PR & (1 << i)) {
            EXTI->PR = (1 << i);
            if (s_generic_handler) s_generic_handler(i);
        }
    }
}

void EXTI15_10_IRQHandler(void) {
    for (uint8_t i = 10; i <= 15; ++i) {
        if (EXTI->PR & (1 << i)) {
            EXTI->PR = (1 << i);
            if (s_generic_handler) s_generic_handler(i);
        }
    }
}
