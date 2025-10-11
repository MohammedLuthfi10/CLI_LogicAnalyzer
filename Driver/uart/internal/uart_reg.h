/**
 * @file      uart_reg.h
 * @brief     Register definitions for the STM32F4xx USART peripheral.
 * @note      This file is MCU-specific and should only be included by the
 *            corresponding porting file.
 */
#ifndef UART_REG_H
#define UART_REG_H

#include <stdint.h>

// CMSIS-style I/O access qualifiers
#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the STM32F4xx USART peripheral registers.
 */
typedef struct {
    __IO uint32_t SR;   /*!< Offset: 0x00, Status register */
    __IO uint32_t DR;   /*!< Offset: 0x04, Data register */
    __IO uint32_t BRR;  /*!< Offset: 0x08, Baud rate register */
    __IO uint32_t CR1;  /*!< Offset: 0x0C, Control register 1 */
    __IO uint32_t CR2;  /*!< Offset: 0x10, Control register 2 */
    __IO uint32_t CR3;  /*!< Offset: 0x14, Control register 3 */
    __IO uint32_t GTPR; /*!< Offset: 0x18, Guard time and prescaler register */
} uart_reg_map_t;

/* --- Register Bit Field Definitions (SR) --- */
#define USART_SR_CTS_Pos  (9U)
#define USART_SR_CTS_Msk  (1UL << USART_SR_CTS_Pos)
#define USART_SR_LBD_Pos  (8U)
#define USART_SR_LBD_Msk  (1UL << USART_SR_LBD_Pos)
#define USART_SR_TXE_Pos  (7U)
#define USART_SR_TXE_Msk  (1UL << USART_SR_TXE_Pos)
#define USART_SR_TC_Pos   (6U)
#define USART_SR_TC_Msk   (1UL << USART_SR_TC_Pos)
#define USART_SR_RXNE_Pos (5U)
#define USART_SR_RXNE_Msk (1UL << USART_SR_RXNE_Pos)
#define USART_SR_IDLE_Pos (4U)
#define USART_SR_IDLE_Msk (1UL << USART_SR_IDLE_Pos)
#define USART_SR_ORE_Pos  (3U)
#define USART_SR_ORE_Msk  (1UL << USART_SR_ORE_Pos)
#define USART_SR_NE_Pos   (2U)
#define USART_SR_NE_Msk   (1UL << USART_SR_NE_Pos)
#define USART_SR_FE_Pos   (1U)
#define USART_SR_FE_Msk   (1UL << USART_SR_FE_Pos)
#define USART_SR_PE_Pos   (0U)
#define USART_SR_PE_Msk   (1UL << USART_SR_PE_Pos)

/* --- Register Bit Field Definitions (CR1) --- */
#define USART_CR1_OVER8_Pos (15U)
#define USART_CR1_OVER8_Msk (1UL << USART_CR1_OVER8_Pos)
#define USART_CR1_UE_Pos    (13U)
#define USART_CR1_UE_Msk    (1UL << USART_CR1_UE_Pos)
#define USART_CR1_M_Pos     (12U)
#define USART_CR1_M_Msk     (1UL << USART_CR1_M_Pos)
#define USART_CR1_PCE_Pos   (10U)
#define USART_CR1_PCE_Msk   (1UL << USART_CR1_PCE_Pos)
#define USART_CR1_PS_Pos    (9U)
#define USART_CR1_PS_Msk    (1UL << USART_CR1_PS_Pos)
#define USART_CR1_TE_Pos    (3U)
#define USART_CR1_TE_Msk    (1UL << USART_CR1_TE_Pos)
#define USART_CR1_RE_Pos    (2U)
#define USART_CR1_RE_Msk    (1UL << USART_CR1_RE_Pos)

/* --- Register Bit Field Definitions (CR2) --- */
#define USART_CR2_STOP_Pos (12U)
#define USART_CR2_STOP_Msk (3UL << USART_CR2_STOP_Pos)

/* --- Register Bit Field Definitions (CR3) --- */
#define USART_CR3_CTSE_Pos (9U)
#define USART_CR3_CTSE_Msk (1UL << USART_CR3_CTSE_Pos)
#define USART_CR3_RTSE_Pos (8U)
#define USART_CR3_RTSE_Msk (1UL << USART_CR3_RTSE_Pos)

#endif // UART_REG_H
