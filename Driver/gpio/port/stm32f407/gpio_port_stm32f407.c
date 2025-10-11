/**
 * @file      gpio_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/gpio_private.h"
#include "internal/gpio_reg.h"

// Placeholder base addresses
#define AHB1PERIPH_BASE       0x40020000UL
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
//... add other ports as needed

// --- Private function implementations for STM32F4 ---

static void stm32f4_enable_clock(uint8_t port_num) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., RCC->AHB1ENR |= (1 << port_num);
}

static void stm32f4_configure_pins(struct gpio_handle_t* handle) {
    gpio_reg_map_t* gpio_regs = (gpio_reg_map_t*)handle->port_hw_instance;
    const gpio_config_t* config = &handle->config;

    for (uint8_t i = 0; i < 16; ++i) {
        if ((handle->pin_mask >> i) & 1) {
            // --- Configure Mode ---
            uint32_t mode_val = config->mode;
            gpio_regs->MODER &= ~(0b11 << (i * 2));
            gpio_regs->MODER |= (mode_val << (i * 2));

            // --- Configure Pull-up/Pull-down ---
            uint32_t pull_val = config->pull;
            gpio_regs->PUPDR &= ~(0b11 << (i * 2));
            gpio_regs->PUPDR |= (pull_val << (i * 2));

            if (config->mode == GPIO_MODE_OUTPUT |

| config->mode == GPIO_MODE_ALTERNATE_FUNCTION) {
                // --- Configure Output Type ---
                gpio_regs->OTYPER &= ~(1 << i);
                gpio_regs->OTYPER |= (config->output_type << i);

                // --- Configure Speed ---
                uint32_t speed_val = config->speed;
                gpio_regs->OSPEEDR &= ~(0b11 << (i * 2));
                gpio_regs->OSPEEDR |= (speed_val << (i * 2));
            }

            // --- Configure Alternate Function ---
            if (config->mode == GPIO_MODE_ALTERNATE_FUNCTION) {
                uint32_t af_val = config->alternate_function;
                if (i < 8) { // Use AFRL
                    gpio_regs->AFRL &= ~(0b1111 << (i * 4));
                    gpio_regs->AFRL |= (af_val << (i * 4));
                } else { // Use AFRH
                    gpio_regs->AFRH &= ~(0b1111 << ((i - 8) * 4));
                    gpio_regs->AFRH |= (af_val << ((i - 8) * 4));
                }
            }
        }
    }
}

static void stm32f4_set_pins(void* port_hw_instance, uint16_t pin_mask) {
    ((gpio_reg_map_t*)port_hw_instance)->BSRR = pin_mask;
}

static void stm32f4_clear_pins(void* port_hw_instance, uint16_t pin_mask) {
    ((gpio_reg_map_t*)port_hw_instance)->BSRR = (pin_mask << 16);
}

static void stm32f4_toggle_pins(void* port_hw_instance, uint16_t pin_mask) {
    gpio_reg_map_t* gpio_regs = (gpio_reg_map_t*)port_hw_instance;
    uint32_t odr = gpio_regs->ODR;
    gpio_regs->BSRR = ((odr & pin_mask) << 16) | (~odr & pin_mask);
}

static uint16_t stm32f4_read_pins(void* port_hw_instance) {
    return (uint16_t)((gpio_reg_map_t*)port_hw_instance)->IDR;
}

// --- The concrete port interface for STM32F4 ---
static const gpio_port_interface_t stm32f4_port_api = {
   .enable_clock = stm32f4_enable_clock,
   .configure_pins = stm32f4_configure_pins,
   .set_pins = stm32f4_set_pins,
   .clear_pins = stm32f4_clear_pins,
   .toggle_pins = stm32f4_toggle_pins,
   .read_pins = stm32f4_read_pins,
};

// --- Public functions provided by the port ---
const gpio_port_interface_t* gpio_port_get_api(void) {
    return &stm32f4_port_api;
}

void* gpio_port_get_base_addr(uint8_t port_num) {
    switch (port_num) {
        case 0: return (void*)GPIOA_BASE;
        case 1: return (void*)GPIOB_BASE;
        case 2: return (void*)GPIOC_BASE;
        //... add other ports as needed
        default: return NULL;
    }
}
