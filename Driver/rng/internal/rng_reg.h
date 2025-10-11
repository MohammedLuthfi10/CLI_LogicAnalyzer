/**
 * @file      rng_reg.h
 * @brief     Register definitions for the STM32F4xx RNG peripheral.
 */
#ifndef RNG_REG_H
#define RNG_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/** @brief Structure for the RNG peripheral. */
typedef struct {
    __IO uint32_t CR;
    __I  uint32_t SR;
    __I  uint32_t DR;
} rng_reg_map_t;

/* --- Register Bit Field Definitions --- */
#define RNG_CR_RNGEN_Pos    (2U)
#define RNG_CR_RNGEN_Msk    (1UL << RNG_CR_RNGEN_Pos)

#define RNG_SR_DRDY_Pos     (0U)
#define RNG_SR_DRDY_Msk     (1UL << RNG_SR_DRDY_Pos)
#define RNG_SR_CECS_Pos     (1U)
#define RNG_SR_CECS_Msk     (1UL << RNG_SR_CECS_Pos)
#define RNG_SR_SECS_Pos     (2U)
#define RNG_SR_SECS_Msk     (1UL << RNG_SR_SECS_Pos)

#endif // RNG_REG_H
