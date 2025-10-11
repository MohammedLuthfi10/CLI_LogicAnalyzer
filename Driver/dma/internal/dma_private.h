/**
 * @file      dma_private.h
 * @brief     Private internal definitions for the DMA driver.
 */

#ifndef DMA_PRIVATE_H
#define DMA_PRIVATE_H

#include "dma.h"
#include "dma_config.h"
#include "port/dma_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct dma_handle_t {
    const dma_config_t config;
    const uint8_t dma_num;
    const uint8_t stream_num;
    const dma_port_interface_t* port_api;
    void* port_controller_instance; // Pointer to DMA controller (e.g., DMA1)
    void* port_stream_instance;     // Pointer to specific stream (e.g., DMA1_Stream0)
};

#endif // DMA_PRIVATE_H
