/**
 * @file      adc_reg.h
 * @brief     Register definitions for the STM32F4xx ADC peripheral.
 */
#ifndef ADC_REG_H
#define ADC_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the ADC peripheral registers.
 */
typedef struct {
    __IO uint32_t SR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMPR1;
    __IO uint32_t SMPR2;
    __IO uint32_t JOFR1;
    __IO uint32_t JOFR2;
    __IO uint32_t JOFR3;
    __IO uint32_t JOFR4;
    __IO uint32_t HTR;
    __IO uint32_t LTR;
    __IO uint32_t SQR1;
    __IO uint32_t SQR2;
    __IO uint32_t SQR3;
    __IO uint32_t JSQR;
    __IO uint32_t JDR1;
    __IO uint32_t JDR2;
    __IO uint32_t JDR3;
    __IO uint32_t JDR4;
    __IO uint32_t DR;
} adc_reg_map_t;

/**
 * @brief Structure type to access the ADC common registers.
 */
typedef struct {
    __I  uint32_t CSR;
    __IO uint32_t CCR;
    __I  uint32_t CDR;
} adc_common_reg_map_t;

/* --- Register Bit Field Definitions --- */
#define ADC_CR1_RES_Pos     (24U)
#define ADC_CR1_RES_Msk     (3UL << ADC_CR1_RES_Pos)
#define ADC_CR1_SCAN_Pos    (8U)
#define ADC_CR1_SCAN_Msk    (1UL << ADC_CR1_SCAN_Pos)

#define ADC_CR2_SWSTART_Pos (30U)
#define ADC_CR2_SWSTART_Msk (1UL << ADC_CR2_SWSTART_Pos)
#define ADC_CR2_ALIGN_Pos   (11U)
#define ADC_CR2_ALIGN_Msk   (1UL << ADC_CR2_ALIGN_Pos)
#define ADC_CR2_CONT_Pos    (1U)
#define ADC_CR2_CONT_Msk    (1UL << ADC_CR2_CONT_Pos)
#define ADC_CR2_ADON_Pos    (0U)
#define ADC_CR2_ADON_Msk    (1UL << ADC_CR2_ADON_Pos)

#define ADC_SQR1_L_Pos      (20U)
#define ADC_SQR1_L_Msk      (0xFUL << ADC_SQR1_L_Pos)

#define ADC_SR_EOC_Pos      (1U)
#define ADC_SR_EOC_Msk      (1UL << ADC_SR_EOC_Pos)

#define ADC_CCR_ADCPRE_Pos  (16U)
#define ADC_CCR_ADCPRE_Msk  (3UL << ADC_CCR_ADCPRE_Pos)
#define ADC_CCR_TSVREFE_Pos (23U)
#define ADC_CCR_TSVREFE_Msk (1UL << ADC_CCR_TSVREFE_Pos)

#endif // ADC_REG_H
