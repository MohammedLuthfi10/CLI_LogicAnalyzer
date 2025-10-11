/**
 * @file      rtc_reg.h
 * @brief     Register definitions for the STM32F4xx RTC peripheral.
 */
#ifndef RTC_REG_H
#define RTC_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/** @brief Structure for the RTC peripheral. */
typedef struct {
    __IO uint32_t TR;
    __IO uint32_t DR;
    __IO uint32_t CR;
    __IO uint32_t ISR;
    __IO uint32_t PRER;
    __IO uint32_t WUTR;
    __IO uint32_t CALIBR;
    __IO uint32_t ALRMAR;
    __IO uint32_t ALRMBR;
    __O  uint32_t WPR;
} rtc_reg_map_t;

/* --- Register Bit Field Definitions --- */
#define RTC_WPR_KEY1 0xCA
#define RTC_WPR_KEY2 0x53

#define RTC_ISR_INIT_Pos    (7U)
#define RTC_ISR_INIT_Msk    (1UL << RTC_ISR_INIT_Pos)
#define RTC_ISR_INITF_Pos   (6U)
#define RTC_ISR_INITF_Msk   (1UL << RTC_ISR_INITF_Pos)
#define RTC_ISR_RSF_Pos     (5U)
#define RTC_ISR_RSF_Msk     (1UL << RTC_ISR_RSF_Pos)

#define RTC_CR_BYPSHAD_Pos  (5U)
#define RTC_CR_BYPSHAD_Msk  (1UL << RTC_CR_BYPSHAD_Pos)

#define RTC_PRER_PREDIV_A_Pos (16U)
#define RTC_PRER_PREDIV_A_Msk (0x7FUL << RTC_PRER_PREDIV_A_Pos)
#define RTC_PRER_PREDIV_S_Pos (0U)
#define RTC_PRER_PREDIV_S_Msk (0x7FFFUL << RTC_PRER_PREDIV_S_Pos)

#endif // RTC_REG_H
