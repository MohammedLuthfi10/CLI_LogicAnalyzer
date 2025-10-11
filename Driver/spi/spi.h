/**
 * @file      spi.h
 * @brief     Public API for the portable SPI driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions and data structures
 *            for interacting with an SPI peripheral in master mode.
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing an SPI driver instance.
 */
typedef struct spi_handle_t* spi_handle_t;

/* --- Public Configuration Types --- */

/** @brief Baud rate prescaler options. Divides the peripheral clock. */
typedef enum {
    SPI_BAUD_RATE_DIV_2 = 0,
    SPI_BAUD_RATE_DIV_4,
    SPI_BAUD_RATE_DIV_8,
    SPI_BAUD_RATE_DIV_16,
    SPI_BAUD_RATE_DIV_32,
    SPI_BAUD_RATE_DIV_64,
    SPI_BAUD_RATE_DIV_128,
    SPI_BAUD_RATE_DIV_256,
} spi_baud_rate_t;

/** @brief SPI clock polarity (CPOL) setting. */
typedef enum {
    SPI_CLOCK_POLARITY_LOW = 0,  //!< Clock is low when idle.
    SPI_CLOCK_POLARITY_HIGH = 1, //!< Clock is high when idle.
} spi_clock_polarity_t;

/** @brief SPI clock phase (CPHA) setting. */
typedef enum {
    SPI_CLOCK_PHASE_1_EDGE = 0, //!< Data captured on the first clock edge.
    SPI_CLOCK_PHASE_2_EDGE = 1, //!< Data captured on the second clock edge.
} spi_clock_phase_t;

/** @brief Data frame bit order. */
typedef enum {
    SPI_BIT_ORDER_MSB_FIRST = 0, //!< Most significant bit transmitted first.
    SPI_BIT_ORDER_LSB_FIRST = 1, //!< Least significant bit transmitted first.
} spi_bit_order_t;

/**
 * @brief Configuration structure for SPI initialization (Master Mode).
 */
typedef struct {
    spi_baud_rate_t baud_rate_prescaler; //!< Clock speed prescaler.
    spi_clock_polarity_t clock_polarity; //!< Clock polarity (CPOL).
    spi_clock_phase_t clock_phase;       //!< Clock phase (CPHA).
    spi_bit_order_t bit_order;           //!< MSB or LSB first.
} spi_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes an SPI peripheral instance in master mode.
 *
 * @param[in] instance_num The hardware instance number (e.g., 1 for SPI1).
 * @param[in] config Pointer to the user-provided configuration structure.
 *
 * @return A handle to the initialized SPI instance, or NULL on failure.
 */
spi_handle_t spi_init(uint8_t instance_num, const spi_config_t* config);

/**
 * @brief De-initializes an SPI peripheral instance.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 *                         The handle will be set to NULL on success.
 */
void spi_deinit(spi_handle_t* p_handle);

/**
 * @brief Performs a full-duplex block transfer in blocking mode.
 *
 * @details Sends data from `p_tx_data` while simultaneously receiving data
 *          into `p_rx_data`. Both buffers must be of the same length.
 *          If only writing is needed, `p_rx_data` can be NULL.
 *          If only reading is needed, `p_tx_data` can be NULL (dummy bytes,
 *          typically 0xFF, will be sent).
 *
 * @param[in] handle The handle to the SPI instance.
 * @param[in] p_tx_data Pointer to the data buffer to transmit. Can be NULL.
 * @param[out] p_rx_data Pointer to the buffer to store received data. Can be NULL.
 * @param[in] len The number of bytes to transfer.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int spi_transfer_blocking(spi_handle_t handle, const uint8_t* p_tx_data, uint8_t* p_rx_data, size_t len);

#endif // SPI_H
