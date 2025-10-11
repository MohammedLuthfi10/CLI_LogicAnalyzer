/**
 * @file      dac_reg.h
 * @brief     Register definitions for the STM32F4xx DAC peripheral.
 */
#ifndef DAC_REG_H
#define DAC_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the DAC peripheral registers.
 */
typedef struct {
    __IO uint32_t CR;
    __O  uint32_t SWTRIGR;
    __IO uint32_t DHR12R1;
    __IO uint32_t DHR12L1;
    __IO uint32_t DHR8R1;
    __IO uint32_t DHR12R2;
    __IO uint32_t DHR12L2;
    __IO uint32_t DHR8R2;
    __IO uint32_t DHR12RD;
    __IO uint32_t DHR12LD;
    __IO uint32_t DHR8RD;
    __I  uint32_t DOR1;
    __I  uint32_t DOR2;
    __IO uint32_t SR;
} dac_reg_map_t;

/* --- Register Bit Field Definitions --- */
#define DAC_CR_EN1_Pos      (0U)
#define DAC_CR_EN1_Msk      (1UL << DAC_CR_EN1_Pos)
#define DAC_CR_BOFF1_Pos    (1U)
#define DAC_CR_BOFF1_Msk    (1UL << DAC_CR_BOFF1_Pos)
#define DAC_CR_TEN1_Pos     (2U)
#define DAC_CR_TEN1_Msk     (1UL << DAC_CR_TEN1_Pos)
#define DAC_CR_TSEL1_Pos    (3U)
#define DAC_CR_TSEL1_Msk    (7UL << DAC_CR_TSEL1_Pos)

#define DAC_CR_EN2_Pos      (16U)
#define DAC_CR_EN2_Msk      (1UL << DAC_CR_EN2_Pos)
#define DAC_CR_BOFF2_Pos    (17U)
#define DAC_CR_BOFF2_Msk    (1UL << DAC_CR_BOFF2_Pos)
#define DAC_CR_TEN2_Pos     (18U)
#define DAC_CR_TEN2_Msk     (1UL << DAC_CR_TEN2_Pos)
#define DAC_CR_TSEL2_Pos    (19U)
#define DAC_CR_TSEL2_Msk    (7UL << DAC_CR_TSEL2_Pos)

#define DAC_SWTRIGR_SWTRIG1_Pos (0U)
#define DAC_SWTRIGR_SWTRIG1_Msk (1UL << DAC_SWTRIGR_SWTRIG1_Pos)
#define DAC_SWTRIGR_SWTRIG2_Pos (1U)
#define DAC_SWTRIGR_SWTRIG2_Msk (1UL << DAC_SWTRIGR_SWTRIG2_Pos)

#endif // DAC_REG_H
