/**
 * @file      uart_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/uart_private.h"
#include "internal/uart_reg.h"

// These would typically be in a separate, higher-level MCU header
#define PERIPH_BASE           0x40000000UL
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x1000UL)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)
#define USART6_BASE           (APB2PERIPH_BASE + 0x1400UL)

// --- Private function implementations for STM32F4 ---

static void stm32f4_enable(struct uart_handle_t* handle) {
    uart_reg_map_t* uart_regs = (uart_reg_map_t*)handle->port_hw_instance;
    uart_regs->CR1 |= USART_CR1_UE_Msk;
}

static void stm32f4_disable(struct uart_handle_t* handle) {
    uart_reg_map_t* uart_regs = (uart_reg_map_t*)handle->port_hw_instance;
    uart_regs->CR1 &= ~USART_CR1_UE_Msk;
}

static void stm32f4_write_byte_blocking(struct uart_handle_t* handle, uint8_t byte) {
    uart_reg_map_t* uart_regs = (uart_reg_map_t*)handle->port_hw_instance;
    while (!(uart_regs->SR & USART_SR_TXE_Msk));
    uart_regs->DR = byte;
}

static uint8_t stm32f4_read_byte_blocking(struct uart_handle_t* handle) {
    uart_reg_map_t* uart_regs = (uart_reg_map_t*)handle->port_hw_instance;
    while (!(uart_regs->SR & USART_SR_RXNE_Msk));
    return (uint8_t)(uart_regs->DR & 0xFF);
}

static void stm32f4_configure_core(struct uart_handle_t* handle) {
    uart_reg_map_t* uart_regs = (uart_reg_map_t*)handle->port_hw_instance;
    const uart_config_t* config = &handle->config;
    uint32_t cr1 = 0, cr2 = 0, cr3 = 0;

    // Word Length
    if (config->word_length == 9) {
        cr1 |= USART_CR1_M_Msk;
    }

    // Parity
    switch (config->parity) {
        case UART_PARITY_EVEN: cr1 |= USART_CR1_PCE_Msk; break;
        case UART_PARITY_ODD:  cr1 |= (USART_CR1_PCE_Msk | USART_CR1_PS_Msk); break;
        case UART_PARITY_NONE: // Fallthrough
        default: break;
    }

    // Stop Bits
    switch (config->stop_bits) {
        case UART_STOP_BITS_2:   cr2 |= (0b10 << USART_CR2_STOP_Pos); break;
        // Add other cases if needed
        case UART_STOP_BITS_1:   // Fallthrough
        default: break;
    }

    // Flow Control
    switch (config->flow_control) {
        case UART_FLOW_CONTROL_RTS:     cr3 |= USART_CR3_RTSE_Msk; break;
        case UART_FLOW_CONTROL_CTS:     cr3 |= USART_CR3_CTSE_Msk; break;
        case UART_FLOW_CONTROL_RTS_CTS: cr3 |= (USART_CR3_RTSE_Msk | USART_CR3_CTSE_Msk); break;
        case UART_FLOW_CONTROL_NONE:    // Fallthrough
        default: break;
    }

    // Baud Rate
    uint32_t clock_freq = uart_port_get_clock_freq(1); // Assuming instance 1 for now
    uart_regs->BRR = (clock_freq + (config->baud_rate / 2)) / config->baud_rate;

    // Apply configuration
    uart_regs->CR1 = cr1 | USART_CR1_TE_Msk | USART_CR1_RE_Msk;
    uart_regs->CR2 = cr2;
    uart_regs->CR3 = cr3;
}

static void stm32f4_enable_clock(struct uart_handle_t* handle) {
    // NOTE: This is a placeholder. A real implementation would use the RCC
    // peripheral driver to enable the clock for the specific USART instance.
    // For example: rcc_periph_clock_enable(RCC_USART1);
}

static void stm32f4_disable_clock(struct uart_handle_t* handle) {
    // Placeholder for RCC clock disable
}

static void stm32f4_init_pins(struct uart_handle_t* handle) {
    // NOTE: This is a placeholder. A real implementation would use a GPIO
    // driver to configure the TX and RX pins for their alternate function.
}

// --- The concrete port interface for STM32F4 ---
static const uart_port_interface_t stm32f4_port_api = {
   .enable_clock = stm32f4_enable_clock,
   .disable_clock = stm32f4_disable_clock,
   .init_pins = stm32f4_init_pins,
   .configure_core = stm32f4_configure_core,
   .write_byte_blocking = stm32f4_write_byte_blocking,
   .read_byte_blocking = stm32f4_read_byte_blocking,
   .enable = stm32f4_enable,
   .disable = stm32f4_disable,
};

// --- Public functions provided by the port ---
const uart_port_interface_t* uart_port_get_api_for_instance(uint8_t instance_num) {
    // In a real project, you might have different APIs for different USARTs
    switch (instance_num) {
        case 1: // Fallthrough
        case 2: // Fallthrough
        case 6: return &stm32f4_port_api;
        default: return NULL;
    }
}

void* uart_port_get_base_addr_for_instance(uint8_t instance_num) {
    switch (instance_num) {
        case 1: return (void*)USART1_BASE;
        case 2: return (void*)USART2_BASE;
        case 6: return (void*)USART6_BASE;
        default: return NULL;
    }
}

uint32_t uart_port_get_clock_freq(uint8_t instance_num) {
    // NOTE: Placeholder. A real implementation would query the RCC driver
    // to get the actual peripheral clock frequency.
    // Assuming APB2 clock is 84MHz for USART1/6 and APB1 is 42MHz for USART2
    if (instance_num == 1 |

| instance_num == 6) {
        return 84000000;
    } else {
        return 42000000;
    }
}
