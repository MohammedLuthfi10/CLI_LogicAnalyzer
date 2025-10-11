/**
 * @file      rcc_reg.h
 * @brief     Register definitions for the STM32F4xx RCC peripheral.
 */
#ifndef RCC_REG_H
#define RCC_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the STM32F4xx RCC peripheral registers.
 */
typedef struct {
    __IO uint32_t CR;         /*!< Offset: 0x00, Clock control register */
    __IO uint32_t PLLCFGR;    /*!< Offset: 0x04, PLL Configuration register */
    __IO uint32_t CFGR;       /*!< Offset: 0x08, Clock Configuration register */
    __IO uint32_t CIR;        /*!< Offset: 0x0C, Clock interrupt register */
    __IO uint32_t AHB1RSTR;   /*!< Offset: 0x10, AHB1 peripheral reset register */
    __IO uint32_t AHB2RSTR;   /*!< Offset: 0x14, AHB2 peripheral reset register */
    __IO uint32_t AHB3RSTR;   /*!< Offset: 0x18, AHB3 peripheral reset register */
         uint32_t RESERVED0;
    __IO uint32_t APB1RSTR;   /*!< Offset: 0x20, APB1 peripheral reset register */
    __IO uint32_t APB2RSTR;   /*!< Offset: 0x24, APB2 peripheral reset register */
         uint32_t RESERVED1[1];
    __IO uint32_t AHB1ENR;    /*!< Offset: 0x30, AHB1 peripheral enable register */
    __IO uint32_t AHB2ENR;    /*!< Offset: 0x34, AHB2 peripheral enable register */
    __IO uint32_t AHB3ENR;    /*!< Offset: 0x38, AHB3 peripheral enable register */
         uint32_t RESERVED2;
    __IO uint32_t APB1ENR;    /*!< Offset: 0x40, APB1 peripheral enable register */
    __IO uint32_t APB2ENR;    /*!< Offset: 0x44, APB2 peripheral enable register */
         uint32_t RESERVED3[1];
    __IO uint32_t AHB1LPENR;  /*!< Offset: 0x50, AHB1 peripheral enable in low power register */
    __IO uint32_t AHB2LPENR;  /*!< Offset: 0x54, AHB2 peripheral enable in low power register */
    __IO uint32_t AHB3LPENR;  /*!< Offset: 0x58, AHB3 peripheral enable in low power register */
         uint32_t RESERVED4;
    __IO uint32_t APB1LPENR;  /*!< Offset: 0x60, APB1 peripheral enable in low power register */
    __IO uint32_t APB2LPENR;  /*!< Offset: 0x64, APB2 peripheral enable in low power register */
         uint32_t RESERVED5[1];
    __IO uint32_t BDCR;       /*!< Offset: 0x70, Backup Domain control register */
    __IO uint32_t CSR;        /*!< Offset: 0x74, Clock control and status register */
} rcc_reg_map_t;

/* --- Register Bit Field Definitions --- */
// Add necessary bit definitions here as needed by the port layer, for example:
#define RCC_CR_HSION_Pos        (0U)
#define RCC_CR_HSION_Msk        (1UL << RCC_CR_HSION_Pos)
#define RCC_CR_HSIRDY_Pos       (1U)
#define RCC_CR_HSIRDY_Msk       (1UL << RCC_CR_HSIRDY_Pos)
#define RCC_CR_HSEON_Pos        (16U)
#define RCC_CR_HSEON_Msk        (1UL << RCC_CR_HSEON_Pos)
#define RCC_CR_HSERDY_Pos       (17U)
#define RCC_CR_HSERDY_Msk       (1UL << RCC_CR_HSERDY_Pos)
#define RCC_CR_PLLON_Pos        (24U)
#define RCC_CR_PLLON_Msk        (1UL << RCC_CR_PLLON_Pos)
#define RCC_CR_PLLRDY_Pos       (25U)
#define RCC_CR_PLLRDY_Msk       (1UL << RCC_CR_PLLRDY_Pos)

#define RCC_CFGR_SW_Pos         (0U)
#define RCC_CFGR_SW_Msk         (3UL << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SWS_Pos        (2U)
#define RCC_CFGR_SWS_Msk        (3UL << RCC_CFGR_SWS_Pos)

#endif // RCC_REG_H
