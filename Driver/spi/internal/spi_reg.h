/**
 * @file      spi_reg.h
 * @brief     Register definitions for the STM32F4xx SPI peripheral.
 * @note      This file is MCU-specific.
 */
#ifndef SPI_REG_H
#define SPI_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the STM32F4xx SPI peripheral registers.
 */
typedef struct {
    __IO uint32_t CR1;      /*!< Offset: 0x00, Control register 1 */
    __IO uint32_t CR2;      /*!< Offset: 0x04, Control register 2 */
    __IO uint32_t SR;       /*!< Offset: 0x08, Status register */
    __IO uint32_t DR;       /*!< Offset: 0x0C, Data register */
    __IO uint32_t CRCPR;    /*!< Offset: 0x10, CRC polynomial register */
    __IO uint32_t RXCRCR;   /*!< Offset: 0x14, RX CRC register */
    __IO uint32_t TXCRCR;   /*!< Offset: 0x18, TX CRC register */
    __IO uint32_t I2SCFGR;  /*!< Offset: 0x1C, I2S configuration register */
    __IO uint32_t I2SPR;    /*!< Offset: 0x20, I2S prescaler register */
} spi_reg_map_t;

/* --- Register Bit Field Definitions (CR1) --- */
#define SPI_CR1_BIDIMODE_Pos (15U)
#define SPI_CR1_BIDIMODE_Msk (1UL << SPI_CR1_BIDIMODE_Pos)
#define SPI_CR1_DFF_Pos      (11U)
#define SPI_CR1_DFF_Msk      (1UL << SPI_CR1_DFF_Pos)
#define SPI_CR1_SSM_Pos      (9U)
#define SPI_CR1_SSM_Msk      (1UL << SPI_CR1_SSM_Pos)
#define SPI_CR1_SSI_Pos      (8U)
#define SPI_CR1_SSI_Msk      (1UL << SPI_CR1_SSI_Pos)
#define SPI_CR1_LSBFIRST_Pos (7U)
#define SPI_CR1_LSBFIRST_Msk (1UL << SPI_CR1_LSBFIRST_Pos)
#define SPI_CR1_SPE_Pos      (6U)
#define SPI_CR1_SPE_Msk      (1UL << SPI_CR1_SPE_Pos)
#define SPI_CR1_BR_Pos       (3U)
#define SPI_CR1_BR_Msk       (7UL << SPI_CR1_BR_Pos)
#define SPI_CR1_MSTR_Pos     (2U)
#define SPI_CR1_MSTR_Msk     (1UL << SPI_CR1_MSTR_Pos)
#define SPI_CR1_CPOL_Pos     (1U)
#define SPI_CR1_CPOL_Msk     (1UL << SPI_CR1_CPOL_Pos)
#define SPI_CR1_CPHA_Pos     (0U)
#define SPI_CR1_CPHA_Msk     (1UL << SPI_CR1_CPHA_Pos)

/* --- Register Bit Field Definitions (CR2) --- */
#define SPI_CR2_SSOE_Pos     (2U)
#define SPI_CR2_SSOE_Msk     (1UL << SPI_CR2_SSOE_Pos)

/* --- Register Bit Field Definitions (SR) --- */
#define SPI_SR_BSY_Pos       (7U)
#define SPI_SR_BSY_Msk       (1UL << SPI_SR_BSY_Pos)
#define SPI_SR_TXE_Pos       (1U)
#define SPI_SR_TXE_Msk       (1UL << SPI_SR_TXE_Pos)
#define SPI_SR_RXNE_Pos      (0U)
#define SPI_SR_RXNE_Msk      (1UL << SPI_SR_RXNE_Pos)

#endif // SPI_REG_H
