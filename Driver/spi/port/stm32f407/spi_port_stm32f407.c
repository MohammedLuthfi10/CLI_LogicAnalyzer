/**
 * @file      spi_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/spi_private.h"
#include "internal/spi_reg.h"

// Placeholder base addresses
#define PERIPH_BASE           0x40000000UL
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000UL)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800UL)

// --- Private function implementations for STM32F4 ---

static void stm32f4_enable(struct spi_handle_t* handle) {
    spi_reg_map_t* spi_regs = (spi_reg_map_t*)handle->port_hw_instance;
    spi_regs->CR1 |= SPI_CR1_SPE_Msk;
}

static void stm32f4_disable(struct spi_handle_t* handle) {
    spi_reg_map_t* spi_regs = (spi_reg_map_t*)handle->port_hw_instance;
    spi_regs->CR1 &= ~SPI_CR1_SPE_Msk;
}

static uint8_t stm32f4_transfer_byte(struct spi_handle_t* handle, uint8_t tx_byte) {
    spi_reg_map_t* spi_regs = (spi_reg_map_t*)handle->port_hw_instance;

    // Wait for TX buffer to be empty
    while (!(spi_regs->SR & SPI_SR_TXE_Msk));
    // Write data
    spi_regs->DR = tx_byte;
    // Wait for RX buffer to be not empty
    while (!(spi_regs->SR & SPI_SR_RXNE_Msk));
    // Read received data
    return (uint8_t)spi_regs->DR;
}

static void stm32f4_configure_core(struct spi_handle_t* handle) {
    spi_reg_map_t* spi_regs = (spi_reg_map_t*)handle->port_hw_instance;
    const spi_config_t* config = &handle->config;
    uint32_t cr1 = 0;

    // Set master mode, software slave management, and internal slave select
    cr1 |= SPI_CR1_MSTR_Msk | SPI_CR1_SSM_Msk | SPI_CR1_SSI_Msk;

    // Baud Rate Prescaler
    cr1 |= (config->baud_rate_prescaler << SPI_CR1_BR_Pos);

    // Clock Polarity and Phase
    cr1 |= (config->clock_polarity << SPI_CR1_CPOL_Pos);
    cr1 |= (config->clock_phase << SPI_CR1_CPHA_Pos);

    // Bit Order
    if (config->bit_order == SPI_BIT_ORDER_LSB_FIRST) {
        cr1 |= SPI_CR1_LSBFIRST_Msk;
    }

    // Set 8-bit data frame format (default)
    cr1 &= ~SPI_CR1_DFF_Msk;

    // Apply configuration
    spi_regs->CR1 = cr1;

    // Enable SS output if using hardware NSS
    // For this driver, we default to software NSS management
    // spi_regs->CR2 |= SPI_CR2_SSOE_Msk;
}

static void stm32f4_enable_clock(struct spi_handle_t* handle) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., rcc_periph_clock_enable(RCC_SPI1);
}

static void stm32f4_init_pins(struct spi_handle_t* handle) {
    // Placeholder: A real implementation would use a GPIO driver to
    // configure SCK, MISO, MOSI pins for their alternate function.
}

// --- The concrete port interface for STM32F4 ---
static const spi_port_interface_t stm32f4_port_api = {
  .enable_clock = stm32f4_enable_clock,
  .init_pins = stm32f4_init_pins,
  .configure_core = stm32f4_configure_core,
  .transfer_byte = stm32f4_transfer_byte,
  .enable = stm32f4_enable,
  .disable = stm32f4_disable,
};

// --- Public functions provided by the port ---
const spi_port_interface_t* spi_port_get_api_for_instance(uint8_t instance_num) {
    switch (instance_num) {
        case 1: // Fallthrough
        case 2: return &stm32f4_port_api;
        default: return NULL;
    }
}

void* spi_port_get_base_addr_for_instance(uint8_t instance_num) {
    switch (instance_num) {
        case 1: return (void*)SPI1_BASE;
        case 2: return (void*)SPI2_BASE;
        default: return NULL;
    }
}
