/**
 * @file      dma.h
 * @brief     Public API for the portable DMA driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for configuring and
 *            managing a DMA stream for data transfers.
 */

#ifndef DMA_H
#define DMA_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing a configured DMA stream.
 */
typedef struct dma_handle_t* dma_handle_t;

/* --- Public Configuration Types --- */

/** @brief DMA stream priority level. */
typedef enum {
    DMA_PRIORITY_LOW = 0,
    DMA_PRIORITY_MEDIUM,
    DMA_PRIORITY_HIGH,
    DMA_PRIORITY_VERY_HIGH,
} dma_priority_t;

/** @brief DMA data transfer direction. */
typedef enum {
    DMA_DIRECTION_PERIPHERAL_TO_MEMORY = 0,
    DMA_DIRECTION_MEMORY_TO_PERIPHERAL,
    DMA_DIRECTION_MEMORY_TO_MEMORY,
} dma_direction_t;

/** @brief DMA data size for memory and peripheral. */
typedef enum {
    DMA_DATA_SIZE_8_BIT = 0,
    DMA_DATA_SIZE_16_BIT,
    DMA_DATA_SIZE_32_BIT,
} dma_data_size_t;

/** @brief DMA interrupt types. */
typedef enum {
    DMA_INTERRUPT_TRANSFER_COMPLETE,
    DMA_INTERRUPT_HALF_TRANSFER,
    DMA_INTERRUPT_TRANSFER_ERROR,
} dma_interrupt_t;

/**
 * @brief Configuration structure for DMA stream initialization.
 */
typedef struct {
    uint8_t channel;                  // Hardware channel selection (0-7)
    dma_direction_t direction;        // Transfer direction
    dma_priority_t priority;          // Stream priority
    dma_data_size_t peripheral_data_size;
    dma_data_size_t memory_data_size;
    bool peripheral_increment;
    bool memory_increment;
    bool circular_mode;
} dma_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes and configures a DMA stream.
 *
 * @param[in] dma_num The DMA controller number (e.g., 1 for DMA1).
 * @param[in] stream_num The stream number (0-7).
 * @param[in] config Pointer to the configuration structure for the stream.
 *
 * @return A handle to the configured DMA stream, or NULL on failure.
 */
dma_handle_t dma_init(uint8_t dma_num, uint8_t stream_num, const dma_config_t* config);

/**
 * @brief De-initializes a DMA stream.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void dma_deinit(dma_handle_t* p_handle);

/**
 * @brief Starts a DMA transfer.
 *
 * @param[in] handle The handle to the DMA stream.
 * @param[in] source_address The source address for the transfer.
 * @param[in] destination_address The destination address for the transfer.
 * @param[in] data_count The number of data items to transfer.
 */
void dma_start_transfer(dma_handle_t handle, const void* source_address, void* destination_address, uint16_t data_count);

/**
 * @brief Stops the currently active DMA transfer.
 * @param[in] handle The handle to the DMA stream.
 */
void dma_stop_transfer(dma_handle_t handle);

/**
 * @brief Enables a specific DMA interrupt.
 * @param[in] handle The handle to the DMA stream.
 * @param[in] interrupt The interrupt type to enable.
 */
void dma_enable_interrupt(dma_handle_t handle, dma_interrupt_t interrupt);

/**
 * @brief Checks if a specific DMA interrupt flag is set.
 * @param[in] handle The handle to the DMA stream.
 * @param[in] interrupt The interrupt type to check.
 * @return true if the flag is set, false otherwise.
 */
bool dma_is_interrupt_flag_set(dma_handle_t handle, dma_interrupt_t interrupt);

/**
 * @brief Clears a specific DMA interrupt flag.
 * @param[in] handle The handle to the DMA stream.
 * @param[in] interrupt The interrupt type to clear.
 */
void dma_clear_interrupt_flag(dma_handle_t handle, dma_interrupt_t interrupt);

#endif // DMA_H
