/**
 * @file      flash_reg.h
 * @brief     Register definitions for the STM32F4xx FLASH controller.
 */
#ifndef FLASH_REG_H
#define FLASH_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the FLASH peripheral registers.
 */
typedef struct {
    __IO uint32_t ACR;
    __O  uint32_t KEYR;
    __O  uint32_t OPTKEYR;
    __IO uint32_t SR;
    __IO uint32_t CR;
    __IO uint32_t OPTCR;
} flash_reg_map_t;

/* --- Register Bit Field Definitions --- */
#define FLASH_ACR_LATENCY_Pos   (0U)
#define FLASH_ACR_LATENCY_Msk   (0xFUL << FLASH_ACR_LATENCY_Pos)

#define FLASH_SR_BSY_Pos        (16U)
#define FLASH_SR_BSY_Msk        (1UL << FLASH_SR_BSY_Pos)
#define FLASH_SR_EOP_Pos        (0U)
#define FLASH_SR_EOP_Msk        (1UL << FLASH_SR_EOP_Pos)

#define FLASH_CR_LOCK_Pos       (31U)
#define FLASH_CR_LOCK_Msk       (1UL << FLASH_CR_LOCK_Pos)
#define FLASH_CR_STRT_Pos       (16U)
#define FLASH_CR_STRT_Msk       (1UL << FLASH_CR_STRT_Pos)
#define FLASH_CR_PSIZE_Pos      (8U)
#define FLASH_CR_PSIZE_Msk      (3UL << FLASH_CR_PSIZE_Pos)
#define FLASH_CR_SNB_Pos        (3U)
#define FLASH_CR_SNB_Msk        (0x1FUL << FLASH_CR_SNB_Pos)
#define FLASH_CR_MER_Pos        (2U)
#define FLASH_CR_MER_Msk        (1UL << FLASH_CR_MER_Pos)
#define FLASH_CR_SER_Pos        (1U)
#define FLASH_CR_SER_Msk        (1UL << FLASH_CR_SER_Pos)
#define FLASH_CR_PG_Pos         (0U)
#define FLASH_CR_PG_Msk         (1UL << FLASH_CR_PG_Pos)

#define FLASH_KEYR_KEY1         ((uint32_t)0x45670123)
#define FLASH_KEYR_KEY2         ((uint32_t)0xCDEF89AB)

#endif // FLASH_REG_H
