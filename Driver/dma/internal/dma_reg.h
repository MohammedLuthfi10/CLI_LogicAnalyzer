/**
 * @file      dma_reg.h
 * @brief     Register definitions for the STM32F4xx DMA peripheral.
 */
#ifndef DMA_REG_H
#define DMA_REG_H

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile
#else
  #define   __I     volatile const
#endif
#define     __O     volatile
#define     __IO    volatile

/** @brief Structure for a single DMA stream. */
typedef struct {
    __IO uint32_t CR;
    __IO uint32_t NDTR;
    __IO uint32_t PAR;
    __IO uint32_t M0AR;
    __IO uint32_t M1AR;
    __IO uint32_t FCR;
} dma_stream_reg_map_t;

/** @brief Structure for a full DMA controller. */
typedef struct {
    __I  uint32_t LISR;
    __I  uint32_t HISR;
    __O  uint32_t LIFCR;
    __O  uint32_t HIFCR;
    dma_stream_reg_map_t S[1];
} dma_controller_reg_map_t;

/* --- Register Bit Field Definitions --- */
#define DMA_SxCR_EN_Pos         (0U)
#define DMA_SxCR_EN_Msk         (1UL << DMA_SxCR_EN_Pos)
#define DMA_SxCR_TCIE_Pos       (4U)
#define DMA_SxCR_TCIE_Msk       (1UL << DMA_SxCR_TCIE_Pos)
#define DMA_SxCR_HTIE_Pos       (3U)
#define DMA_SxCR_HTIE_Msk       (1UL << DMA_SxCR_HTIE_Pos)
#define DMA_SxCR_TEIE_Pos       (2U)
#define DMA_SxCR_TEIE_Msk       (1UL << DMA_SxCR_TEIE_Pos)
#define DMA_SxCR_CHSEL_Pos      (25U)
#define DMA_SxCR_CHSEL_Msk      (7UL << DMA_SxCR_CHSEL_Pos)
#define DMA_SxCR_PL_Pos         (16U)
#define DMA_SxCR_PL_Msk         (3UL << DMA_SxCR_PL_Pos)
#define DMA_SxCR_DIR_Pos        (6U)
#define DMA_SxCR_DIR_Msk        (3UL << DMA_SxCR_DIR_Pos)
#define DMA_SxCR_CIRC_Pos       (8U)
#define DMA_SxCR_CIRC_Msk       (1UL << DMA_SxCR_CIRC_Pos)
#define DMA_SxCR_PINC_Pos       (9U)
#define DMA_SxCR_PINC_Msk       (1UL << DMA_SxCR_PINC_Pos)
#define DMA_SxCR_MINC_Pos       (10U)
#define DMA_SxCR_MINC_Msk       (1UL << DMA_SxCR_MINC_Pos)
#define DMA_SxCR_PSIZE_Pos      (11U)
#define DMA_SxCR_PSIZE_Msk      (3UL << DMA_SxCR_PSIZE_Pos)
#define DMA_SxCR_MSIZE_Pos      (13U)
#define DMA_SxCR_MSIZE_Msk      (3UL << DMA_SxCR_MSIZE_Pos)

#endif // DMA_REG_H
