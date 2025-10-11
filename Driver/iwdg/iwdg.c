/**
 * @file      iwdg.c
 * @brief     Hardware-agnostic implementation of the IWDG driver.
 */

#include "internal/iwdg_private.h"
#include <string.h>

// --- Static Data ---
// Since there's only one IWDG, we can use a single static handle.
static struct iwdg_handle_t s_handle;
static bool s_is_handle_initialized = false;

// --- Public API Function Implementations ---

iwdg_handle_t iwdg_init(const iwdg_config_t* config) {
    if (config == NULL |

| s_is_handle_initialized) {
        return NULL;
    }

    s_handle.port_api = iwdg_port_get_api();
    s_handle.port_hw_instance = iwdg_port_get_base_addr();

    if (s_handle.port_api == NULL |

| s_handle.port_hw_instance == NULL) {
        return NULL;
    }

    s_handle.port_api->start_and_configure(&s_handle, config->timeout_ms);

    s_is_handle_initialized = true;
    return &s_handle;
}

void iwdg_deinit(iwdg_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        // The IWDG cannot be stopped. We just invalidate the handle.
        s_is_handle_initialized = false;
        *p_handle = NULL;
    }
}

void iwdg_feed(iwdg_handle_t handle) {
    if (handle && s_is_handle_initialized) {
        handle->port_api->feed(handle);
    }
}
