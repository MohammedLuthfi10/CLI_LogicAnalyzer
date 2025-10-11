/**
 * @file      iwdg_reg.h
 * @brief     Register definitions for the STM32F4xx IWDG peripheral.
 */
#ifndef IWDG_REG_H
#define IWDG_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the IWDG peripheral registers.
 */
typedef struct {
    __O  uint32_t KR;
    __IO uint32_t PR;
    __IO uint32_t RLR;
    __I  uint32_t SR;
} iwdg_reg_map_t;

/* --- Register Key Values --- */
#define IWDG_KR_START   0xCCCC
#define IWDG_KR_UNLOCK  0x5555
#define IWDG_KR_RESET   0xAAAA

/* --- Register Bit Field Definitions --- */
#define IWDG_SR_PVU_Pos (0U)
#define IWDG_SR_PVU_Msk (1UL << IWDG_SR_PVU_Pos)
#define IWDG_SR_RVU_Pos (1U)
#define IWDG_SR_RVU_Msk (1UL << IWDG_SR_RVU_Pos)

#endif // IWDG_REG_H
