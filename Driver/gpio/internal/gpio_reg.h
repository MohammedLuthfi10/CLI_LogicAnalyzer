/**
 * @file      gpio_reg.h
 * @brief     Register definitions for the STM32F4xx GPIO peripheral.
 */
#ifndef GPIO_REG_H
#define GPIO_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the STM32F4xx GPIO peripheral registers.
 */
typedef struct {
    __IO uint32_t MODER;    /*!< Offset: 0x00, Port mode register */
    __IO uint32_t OTYPER;   /*!< Offset: 0x04, Port output type register */
    __IO uint32_t OSPEEDR;  /*!< Offset: 0x08, Port output speed register */
    __IO uint32_t PUPDR;    /*!< Offset: 0x0C, Port pull-up/pull-down register */
    __I  uint32_t IDR;      /*!< Offset: 0x10, Port input data register */
    __IO uint32_t ODR;      /*!< Offset: 0x14, Port output data register */
    __O  uint32_t BSRR;     /*!< Offset: 0x18, Port bit set/reset register */
    __IO uint32_t LCKR;     /*!< Offset: 0x1C, Port configuration lock register */
    __IO uint32_t AFRL;     /*!< Offset: 0x20, Alternate function low register */
    __IO uint32_t AFRH;     /*!< Offset: 0x24, Alternate function high register */
} gpio_reg_map_t;

#endif // GPIO_REG_H
