/**
 * @file      flash.h
 * @brief     Public API for the portable FLASH memory controller driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for erasing and
 *            programming the internal FLASH memory.
 */

#ifndef FLASH_H
#define FLASH_H

#include <stdint.hh>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing the FLASH controller instance.
 */
typedef struct flash_handle_t* flash_handle_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes the FLASH controller interface.
 * @details This function should be called once to get a handle to the controller.
 *
 * @return A handle to the FLASH controller, or NULL on failure.
 */
flash_handle_t flash_init(void);

/**
 * @brief De-initializes the FLASH controller handle.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void flash_deinit(flash_handle_t* p_handle);

/**
 * @brief Sets the FLASH memory wait states (latency).
 * @details This is a critical function that must be called to match the FLASH
 *          access time to the system clock frequency. It should be called
 *          before increasing the system clock speed.
 *
 * @param[in] handle The handle to the FLASH controller.
 * @param[in] system_clock_hz The target system clock frequency in Hz.
 *
 * @return 0 on success, or a negative error code if the frequency is unsupported.
 */
int flash_set_wait_states(flash_handle_t handle, uint32_t system_clock_hz);

/**
 * @brief Erases a single sector of the FLASH memory.
 * @warning This is a blocking operation that can take a significant amount of time.
 *
 * @param[in] handle The handle to the FLASH controller.
 * @param[in] sector_index The index of the sector to erase (e.g., 0, 1, 2...).
 *
 * @return 0 on success, or a negative error code on failure.
 */
int flash_erase_sector(flash_handle_t handle, uint8_t sector_index);

/**
 * @brief Programs a block of data into FLASH memory.
 * @warning This is a blocking operation. The target memory area must be
 *          erased before programming.
 *
 * @param[in] handle The handle to the FLASH controller.
 * @param[in] address The starting address in FLASH to program.
 * @param[in] p_data Pointer to the data buffer to write.
 * @param[in] len The number of bytes to write.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int flash_program(flash_handle_t handle, uint32_t address, const uint8_t* p_data, size_t len);

#endif // FLASH_H
