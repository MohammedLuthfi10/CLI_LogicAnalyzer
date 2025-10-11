/**
 * @file      pwr_reg.h
 * @brief     Register definitions for the STM32F4xx PWR peripheral.
 */
#ifndef PWR_REG_H
#define PWR_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/** @brief Structure for the PWR peripheral. */
typedef struct {
    __IO uint32_t CR;
    __IO uint32_t CSR;
} pwr_reg_map_t;

/** @brief Structure for the System Control Block (SCB). */
typedef struct {
    __I  uint32_t CPUID;
    __IO uint32_t ICSR;
    __IO uint32_t VTOR;
    __IO uint32_t AIRCR;
    __IO uint32_t SCR;
    __IO uint32_t CCR;
    __I  uint32_t SHPR[1];
    __IO uint32_t SHCSR;
} scb_reg_map_t;


/* --- Register Bit Field Definitions --- */
#define PWR_CR_DBP_Pos      (8U)
#define PWR_CR_DBP_Msk      (1UL << PWR_CR_DBP_Pos)
#define PWR_CR_VOS_Pos      (14U)
#define PWR_CR_VOS_Msk      (3UL << PWR_CR_VOS_Pos)
#define PWR_CR_PDDS_Pos     (1U)
#define PWR_CR_PDDS_Msk     (1UL << PWR_CR_PDDS_Pos)
#define PWR_CR_LPDS_Pos     (0U)
#define PWR_CR_LPDS_Msk     (1UL << PWR_CR_LPDS_Pos)
#define PWR_CR_CSBF_Pos     (3U)
#define PWR_CR_CSBF_Msk     (1UL << PWR_CR_CSBF_Pos)

#define SCB_SCR_SLEEPDEEP_Pos (2U)
#define SCB_SCR_SLEEPDEEP_Msk (1UL << SCB_SCR_SLEEPDEEP_Pos)

#endif // PWR_REG_H
