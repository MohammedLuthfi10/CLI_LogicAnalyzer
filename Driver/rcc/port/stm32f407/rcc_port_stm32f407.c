/**
 * @file      rcc_port_stm32f407.c
 * @brief     Concrete porting layer for the STM32F407 series RCC.
 */
#include "internal/rcc_private.h"
#include "internal/rcc_reg.h"

/* --- Base Addresses & Register Pointers --- */
#define PERIPH_BASE           (0x40000000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
#define RCC                   ((rcc_reg_map_t *)RCC_BASE)

// Flash ACR for setting latency
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00UL)
#define FLASH_ACR             (*((__IO uint32_t *)FLASH_R_BASE))

// Stores configured clock frequencies
static rcc_sysclk_config_t s_clk_config = {0};

/* --- Private Port Functions --- */

static int stm32f4_sysclk_init(const rcc_sysclk_config_t* config) {
    // 1. Enable HSE and wait for it to be ready
    RCC->CR |= RCC_CR_HSEON_Msk;
    while (!(RCC->CR & RCC_CR_HSERDY_Msk));

    // 2. Configure AHB, APB1, APB2 prescalers
    // AHB Prescaler = 1 (Max 168MHz)
    RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
    // APB1 Prescaler = 4 (Max 42MHz)
    RCC->CFGR |= (0b101 << RCC_CFGR_PPRE1_Pos);
    // APB2 Prescaler = 2 (Max 84MHz)
    RCC->CFGR |= (0b100 << RCC_CFGR_PPRE2_Pos);

    // 3. Configure the main PLL
    // Source must be HSE, desired PLL VCO output is SYSCLK * PLLP
    // PLL VCO input frequency should be between 1 and 2 MHz. We aim for 1 MHz.
    uint32_t pllm = config->hse_hz / 1000000;
    uint32_t pllp_val = 2; // Fixed PLLP = 2 for max SYSCLK
    uint32_t plln = (config->sysclk_hz * pllp_val) / (config->hse_hz / pllm);

    // PLLP bits are encoded: 00=/2, 01=/4, 10=/6, 11=/8
    uint32_t pllp_bits = (pllp_val / 2) - 1;

    RCC->PLLCFGR = (pllm << RCC_PLLCFGR_PLLM_Pos) |
                   (plln << RCC_PLLCFGR_PLLN_Pos) |
                   (pllp_bits << RCC_PLLCFGR_PLLP_Pos) |
                   RCC_PLLCFGR_PLLSRC_Msk; // Source HSE

    // 4. Enable the PLL and wait for it to be ready
    RCC->CR |= RCC_CR_PLLON_Msk;
    while (!(RCC->CR & RCC_CR_PLLRDY_Msk));

    // 5. Configure Flash latency before switching to a higher clock
    // For VCORE=1.2V (default), up to 168MHz needs 5 wait states
    FLASH_ACR |= 5;

    // 6. Select the PLL as the system clock source
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | (0b10 << RCC_CFGR_SW_Pos);
    while (((RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos) != 0b10);

    // Store configuration for later use
    s_clk_config = *config;
    return 0;
}

static void stm32f4_periph_clock_enable(rcc_periph_t periph) {
    switch (periph) {
        // AHB1
        case RCC_PERIPH_GPIOA: RCC->AHB1ENR |= (1 << 0); break;
        case RCC_PERIPH_GPIOB: RCC->AHB1ENR |= (1 << 1); break;
        case RCC_PERIPH_GPIOC: RCC->AHB1ENR |= (1 << 2); break;
        case RCC_PERIPH_GPIOD: RCC->AHB1ENR |= (1 << 3); break;
        // APB1
        case RCC_PERIPH_TIM2:   RCC->APB1ENR |= (1 << 0); break;
        case RCC_PERIPH_TIM3:   RCC->APB1ENR |= (1 << 1); break;
        case RCC_PERIPH_TIM4:   RCC->APB1ENR |= (1 << 2); break;
        case RCC_PERIPH_TIM5:   RCC->APB1ENR |= (1 << 3); break;
        case RCC_PERIPH_SPI2:   RCC->APB1ENR |= (1 << 14); break;
        case RCC_PERIPH_SPI3:   RCC->APB1ENR |= (1 << 15); break;
        case RCC_PERIPH_USART2: RCC->APB1ENR |= (1 << 17); break;
        case RCC_PERIPH_I2C1:   RCC->APB1ENR |= (1 << 21); break;
        case RCC_PERIPH_I2C2:   RCC->APB1ENR |= (1 << 22); break;
        case RCC_PERIPH_I2C3:   RCC->APB1ENR |= (1 << 23); break;
        // APB2
        case RCC_PERIPH_SPI1:   RCC->APB2ENR |= (1 << 12); break;
        case RCC_PERIPH_USART1: RCC->APB2ENR |= (1 << 4); break;
        case RCC_PERIPH_USART6: RCC->APB2ENR |= (1 << 5); break;
    }
}

static void stm32f4_periph_clock_disable(rcc_periph_t periph) {
    // Similar to enable, but with &= ~
}

static uint32_t stm32f4_get_ahb_clk_freq(void) {
    // For this simple driver, we assume AHB prescaler is 1
    return s_clk_config.sysclk_hz;
}

static uint32_t stm32f4_get_apb1_clk_freq(void) {
    // For this simple driver, we assume APB1 prescaler is 4
    return s_clk_config.sysclk_hz / 4;
}

static uint32_t stm32f4_get_apb2_clk_freq(void) {
    // For this simple driver, we assume APB2 prescaler is 2
    return s_clk_config.sysclk_hz / 2;
}

static const rcc_port_interface_t stm32f4_port_api = {
    .sysclk_init = stm32f4_sysclk_init,
    .periph_clock_enable = stm32f4_periph_clock_enable,
    .periph_clock_disable = stm32f4_periph_clock_disable,
    .get_ahb_clk_freq = stm32f4_get_ahb_clk_freq,
    .get_apb1_clk_freq = stm32f4_get_apb1_clk_freq,
    .get_apb2_clk_freq = stm32f4_get_apb2_clk_freq,
};

const rcc_port_interface_t* rcc_port_get_api(void) {
    return &stm32f4_port_api;
}
