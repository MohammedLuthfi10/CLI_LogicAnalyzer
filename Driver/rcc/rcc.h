/**
 * @file      rcc.h
 * @brief     Public API for the portable Reset and Clock Control (RCC) driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file provides a high-level interface for initializing the
 *            system clock and managing peripheral clocks.
 */

#ifndef RCC_H
#define RCC_H

#include <stdint.h>
#include <stdbool.h>

/* --- Public Configuration Types --- */

/** @brief Defines the primary clock source for the system. */
typedef enum {
    RCC_CLOCK_SOURCE_INTERNAL, // HSI
    RCC_CLOCK_SOURCE_EXTERNAL, // HSE
} rcc_clock_source_t;

/** @brief Defines standard system clock (SYSCLK) frequencies. */
typedef enum {
    RCC_SYSCLK_FREQ_84_MHZ,
    RCC_SYSCLK_FREQ_96_MHZ,
    RCC_SYSCLK_FREQ_168_MHZ,
    RCC_SYSCLK_FREQ_180_MHZ,
} rcc_sysclk_freq_t;

/**
 * @brief Generic peripheral identifiers used for enabling/disabling clocks.
 * @note This enum is platform-agnostic. The port layer maps these IDs to
 *       the appropriate hardware registers and bits.
 */
typedef enum {
    // AHB1
    PERIPH_ID_GPIOA, PERIPH_ID_GPIOB, PERIPH_ID_GPIOC, //... and other GPIOs
    PERIPH_ID_CRC,
    PERIPH_ID_DMA1, PERIPH_ID_DMA2,
    // APB1
    PERIPH_ID_TIM2, PERIPH_ID_TIM3, PERIPH_ID_TIM4, PERIPH_ID_TIM5,
    PERIPH_ID_WWDG,
    PERIPH_ID_SPI2, PERIPH_ID_SPI3,
    PERIPH_ID_USART2,
    PERIPH_ID_I2C1, PERIPH_ID_I2C2, PERIPH_ID_I2C3,
    PERIPH_ID_PWR,
    // APB2
    PERIPH_ID_TIM1, PERIPH_ID_TIM8,
    PERIPH_ID_USART1, PERIPH_ID_USART6,
    PERIPH_ID_ADC1,
    PERIPH_ID_SDIO,
    PERIPH_ID_SPI1,
    PERIPH_ID_SYSCFG,
} peripheral_id_t;


/* --- Public API Functions --- */

/**
 * @brief Initializes the system clock tree (PLL, dividers).
 *
 * @details This function configures the main system clock (SYSCLK), HCLK,
 *          APB1, and APB2 clocks based on the desired frequency. It should be
 *          one of the first functions called in main().
 *
 * @param[in] clk_source The primary clock source (internal HSI or external HSE).
 * @param[in] external_crystal_hz The frequency of the external crystal in Hz.
 *                                Ignored if clk_source is INTERNAL.
 * @param[in] sysclk_freq The desired target system clock frequency.
 *
 * @return true on success, false if the configuration is not possible.
 */
bool rcc_system_init(rcc_clock_source_t clk_source, uint32_t external_crystal_hz, rcc_sysclk_freq_t sysclk_freq);

/**
 * @brief Enables the clock for a specific peripheral.
 * @param[in] id The generic identifier of the peripheral.
 */
void rcc_enable_peripheral_clock(peripheral_id_t id);

/**
 * @brief Disables the clock for a specific peripheral.
 * @param[in] id The generic identifier of the peripheral.
 */
void rcc_disable_peripheral_clock(peripheral_id_t id);

/** @brief Gets the configured AHB bus frequency (HCLK). */
uint32_t rcc_get_ahb_frequency(void);

/** @brief Gets the configured APB1 bus frequency (PCLK1). */
uint32_t rcc_get_apb1_frequency(void);

/** @brief Gets the configured APB2 bus frequency (PCLK2). */
uint32_t rcc_get_apb2_frequency(void);

#endif // RCC_H
