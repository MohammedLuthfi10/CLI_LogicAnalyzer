/**
 * @file      rng.c
 * @brief     Hardware-agnostic implementation of the RNG driver.
 */

#include "internal/rng_private.h"
#include <string.h>

// --- Static Data ---
static struct rng_handle_t s_handle;
static bool s_is_handle_initialized = false;

// --- Public API Function Implementations ---

rng_handle_t rng_init(void) {
    if (s_is_handle_initialized) {
        return &s_handle;
    }

    s_handle.port_api = rng_port_get_api();
    s_handle.port_hw_instance = rng_port_get_base_addr();

    if (s_handle.port_api == NULL |

| s_handle.port_hw_instance == NULL) {
        return NULL;
    }

    s_handle.port_api->enable_clock();
    s_handle.port_api->enable(&s_handle);

    s_is_handle_initialized = true;
    return &s_handle;
}

void rng_deinit(rng_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        (*p_handle)->port_api->disable(*p_handle);
        s_is_handle_initialized = false;
        *p_handle = NULL;
    }
}

int rng_get_random_number(rng_handle_t handle, uint32_t* p_random_number) {
    if (handle && s_is_handle_initialized && p_random_number) {
        return handle->port_api->get_random_blocking(handle, p_random_number);
    }
    return -1;
}
