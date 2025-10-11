/**
 * @file      i2c_reg.h
 * @brief     Register definitions for the STM32F4xx I2C peripheral.
 */
#ifndef I2C_REG_H
#define I2C_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/**
 * @brief Structure type to access the STM32F4xx I2C peripheral registers.
 */
typedef struct {
    __IO uint32_t CR1;      /*!< Offset: 0x00, Control register 1 */
    __IO uint32_t CR2;      /*!< Offset: 0x04, Control register 2 */
    __IO uint32_t OAR1;     /*!< Offset: 0x08, Own address register 1 */
    __IO uint32_t OAR2;     /*!< Offset: 0x0C, Own address register 2 */
    __IO uint32_t DR;       /*!< Offset: 0x10, Data register */
    __IO uint32_t SR1;      /*!< Offset: 0x14, Status register 1 */
    __IO uint32_t SR2;      /*!< Offset: 0x18, Status register 2 */
    __IO uint32_t CCR;      /*!< Offset: 0x1C, Clock control register */
    __IO uint32_t TRISE;    /*!< Offset: 0x20, TRISE register */
    __IO uint32_t FLTR;     /*!< Offset: 0x24, FLTR register */
} i2c_reg_map_t;

/* --- Register Bit Field Definitions (CR1) --- */
#define I2C_CR1_SWRST_Pos   (15U)
#define I2C_CR1_SWRST_Msk   (1UL << I2C_CR1_SWRST_Pos)
#define I2C_CR1_ACK_Pos     (10U)
#define I2C_CR1_ACK_Msk     (1UL << I2C_CR1_ACK_Pos)
#define I2C_CR1_STOP_Pos    (9U)
#define I2C_CR1_STOP_Msk    (1UL << I2C_CR1_STOP_Pos)
#define I2C_CR1_START_Pos   (8U)
#define I2C_CR1_START_Msk   (1UL << I2C_CR1_START_Pos)
#define I2C_CR1_PE_Pos      (0U)
#define I2C_CR1_PE_Msk      (1UL << I2C_CR1_PE_Pos)

/* --- Register Bit Field Definitions (CR2) --- */
#define I2C_CR2_FREQ_Pos    (0U)
#define I2C_CR2_FREQ_Msk    (0x3FUL << I2C_CR2_FREQ_Pos)

/* --- Register Bit Field Definitions (SR1) --- */
#define I2C_SR1_AF_Pos      (10U)
#define I2C_SR1_AF_Msk      (1UL << I2C_SR1_AF_Pos)
#define I2C_SR1_TxE_Pos     (7U)
#define I2C_SR1_TxE_Msk     (1UL << I2C_SR1_TxE_Pos)
#define I2C_SR1_RxNE_Pos    (6U)
#define I2C_SR1_RxNE_Msk    (1UL << I2C_SR1_RxNE_Pos)
#define I2C_SR1_BTF_Pos     (2U)
#define I2C_SR1_BTF_Msk     (1UL << I2C_SR1_BTF_Pos)
#define I2C_SR1_ADDR_Pos    (1U)
#define I2C_SR1_ADDR_Msk    (1UL << I2C_SR1_ADDR_Pos)
#define I2C_SR1_SB_Pos      (0U)
#define I2C_SR1_SB_Msk      (1UL << I2C_SR1_SB_Pos)

/* --- Register Bit Field Definitions (SR2) --- */
#define I2C_SR2_BUSY_Pos    (1U)
#define I2C_SR2_BUSY_Msk    (1UL << I2C_SR2_BUSY_Pos)
#define I2C_SR2_MSL_Pos     (0U)
#define I2C_SR2_MSL_Msk     (1UL << I2C_SR2_MSL_Pos)

/* --- Register Bit Field Definitions (CCR) --- */
#define I2C_CCR_FS_Pos      (15U)
#define I2C_CCR_FS_Msk      (1UL << I2C_CCR_FS_Pos)
#define I2C_CCR_CCR_Pos     (0U)
#define I2C_CCR_CCR_Msk     (0xFFFUL << I2C_CCR_CCR_Pos)

#endif // I2C_REG_H
