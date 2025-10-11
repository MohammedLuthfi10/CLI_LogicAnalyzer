/**
 * @file      flash.c
 * @brief     Hardware-agnostic implementation of the FLASH driver.
 */

#include "internal/flash_private.h"
#include <string.h>

// --- Static Data ---
// Since there's only one FLASH controller, we can use a single static handle.
static struct flash_handle_t s_handle;
static bool s_is_handle_initialized = false;

// --- Public API Function Implementations ---

flash_handle_t flash_init(void) {
    if (s_is_handle_initialized) {
        return &s_handle;
    }

    s_handle.port_api = flash_port_get_api();
    s_handle.port_hw_instance = flash_port_get_base_addr();

    if (s_handle.port_api == NULL || s_handle.port_hw_instance == NULL) {
        return NULL;
    }

    s_is_handle_initialized = true;
    return &s_handle;
}

void flash_deinit(flash_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        // Nothing to de-initialize, just invalidate the handle
        s_is_handle_initialized = false;
        *p_handle = NULL;
    }
}

int flash_set_wait_states(flash_handle_t handle, uint32_t system_clock_hz) {
    if (handle && s_is_handle_initialized) {
        return handle->port_api->set_wait_states(handle, system_clock_hz);
    }
    return -1;
}

int flash_erase_sector(flash_handle_t handle, uint8_t sector_index) {
    if (handle && s_is_handle_initialized) {
        return handle->port_api->erase_sector(handle, sector_index);
    }
    return -1;
}

int flash_program(flash_handle_t handle, uint32_t address, const uint8_t* p_data, size_t len) {
    if (handle && s_is_handle_initialized && p_data!= NULL) {
        return handle->port_api->program(handle, address, p_data, len);
    }
    return -1;
}
