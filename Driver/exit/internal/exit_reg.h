/**
 * @file      exti_reg.h
 * @brief     Register definitions for the STM32F4xx EXTI and SYSCFG peripherals.
 */
#ifndef EXTI_REG_H
#define EXTI_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/** @brief Structure for the EXTI peripheral. */
typedef struct {
    __IO uint32_t IMR;
    __IO uint32_t EMR;
    __IO uint32_t RTSR;
    __IO uint32_t FTSR;
    __IO uint32_t SWIER;
    __IO uint32_t PR;
} exti_reg_map_t;

/** @brief Structure for the SYSCFG peripheral. */
typedef struct {
    __IO uint32_t MEMRMP;
    __IO uint32_t PMC;
    __IO uint32_t EXTICR[1];
    uint32_t      RESERVED[2];
    __IO uint32_t CMPCR;
} syscfg_reg_map_t;

#endif // EXTI_REG_H
