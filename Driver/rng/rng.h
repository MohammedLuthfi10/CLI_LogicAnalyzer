/**
 * @file      rng.h
 * @brief     Public API for the portable Random Number Generator (RNG) driver.
 * @author    Your Name
 * @date      2023-10-27
 */

#ifndef RNG_H
#define RNG_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing the RNG instance.
 */
typedef struct rng_handle_t* rng_handle_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes and enables the RNG peripheral.
 *
 * @return A handle to the RNG instance, or NULL on failure.
 */
rng_handle_t rng_init(void);

/**
 * @brief De-initializes the RNG peripheral.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void rng_deinit(rng_handle_t* p_handle);

/**
 * @brief Gets a 32-bit random number from the hardware generator.
 * @details This is a blocking function that will wait until a random number
 *          is available.
 *
 * @param[in] handle The handle to the RNG instance.
 * @param[out] p_random_number Pointer to a variable to store the generated number.
 *
 * @return 0 on success, or a negative error code on failure (e.g., hardware error).
 */
int rng_get_random_number(rng_handle_t handle, uint32_t* p_random_number);

#endif // RNG_H
