/**
 * @file      timer_reg.h
 * @brief     Register definitions for the STM32F4xx Timer peripherals.
 */
#ifndef TIMER_REG_H
#define TIMER_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access a general-purpose timer (e.g., TIM2-5).
 */
typedef struct {
    __IO uint32_t CR1;      /*!< Offset: 0x00, Control register 1 */
    __IO uint32_t CR2;      /*!< Offset: 0x04, Control register 2 */
    __IO uint32_t SMCR;     /*!< Offset: 0x08, Slave mode control register */
    __IO uint32_t DIER;     /*!< Offset: 0x0C, DMA/Interrupt enable register */
    __IO uint32_t SR;       /*!< Offset: 0x10, Status register */
    __O  uint32_t EGR;      /*!< Offset: 0x14, Event generation register */
    __IO uint32_t CCMR1;    /*!< Offset: 0x18, Capture/compare mode register 1 */
    __IO uint32_t CCMR2;    /*!< Offset: 0x1C, Capture/compare mode register 2 */
    __IO uint32_t CCER;     /*!< Offset: 0x20, Capture/compare enable register */
    __IO uint32_t CNT;      /*!< Offset: 0x24, Counter */
    __IO uint32_t PSC;      /*!< Offset: 0x28, Prescaler */
    __IO uint32_t ARR;      /*!< Offset: 0x2C, Auto-reload register */
    __IO uint32_t RCR;      /*!< Offset: 0x30, Repetition counter register (TIM1/8) */
    __IO uint32_t CCR1;     /*!< Offset: 0x34, Capture/compare register 1 */
    __IO uint32_t CCR2;     /*!< Offset: 0x38, Capture/compare register 2 */
    __IO uint32_t CCR3;     /*!< Offset: 0x3C, Capture/compare register 3 */
    __IO uint32_t CCR4;     /*!< Offset: 0x40, Capture/compare register 4 */
    __IO uint32_t BDTR;     /*!< Offset: 0x44, Break and dead-time register (TIM1/8) */
    __IO uint32_t DCR;      /*!< Offset: 0x48, DMA control register */
    __IO uint32_t DMAR;     /*!< Offset: 0x4C, DMA address for full transfer */
    __IO uint32_t OR;       /*!< Offset: 0x50, Option register */
} timer_reg_map_t;

/* --- Register Bit Field Definitions --- */
#define TIM_CR1_CEN_Pos     (0U)
#define TIM_CR1_CEN_Msk     (1UL << TIM_CR1_CEN_Pos)
#define TIM_CR1_DIR_Pos     (4U)
#define TIM_CR1_DIR_Msk     (1UL << TIM_CR1_DIR_Pos)
#define TIM_CR1_CMS_Pos     (5U)
#define TIM_CR1_CMS_Msk     (3UL << TIM_CR1_CMS_Pos)
#define TIM_CR1_ARPE_Pos    (7U)
#define TIM_CR1_ARPE_Msk    (1UL << TIM_CR1_ARPE_Pos)

#define TIM_DIER_UIE_Pos    (0U)
#define TIM_DIER_UIE_Msk    (1UL << TIM_DIER_UIE_Pos)

#define TIM_SR_UIF_Pos      (0U)
#define TIM_SR_UIF_Msk      (1UL << TIM_SR_UIF_Pos)

#endif // TIMER_REG_H
