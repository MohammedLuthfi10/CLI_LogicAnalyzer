/**
 * @file      pwr.c
 * @brief     Hardware-agnostic implementation of the PWR driver.
 */

#include "internal/pwr_private.h"
#include <string.h>

// --- Static Data ---
// Since there's only one PWR controller, we use a single static handle.
static struct pwr_handle_t s_handle;
static bool s_is_handle_initialized = false;

// --- Public API Function Implementations ---

pwr_handle_t pwr_init(void) {
    if (s_is_handle_initialized) {
        return &s_handle;
    }

    s_handle.port_api = pwr_port_get_api();
    s_handle.port_hw_instance = pwr_port_get_base_addr();

    if (s_handle.port_api == NULL |

| s_handle.port_hw_instance == NULL) {
        return NULL;
    }

    s_handle.port_api->enable_clock();
    s_is_handle_initialized = true;
    return &s_handle;
}

void pwr_deinit(pwr_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        s_is_handle_initialized = false;
        *p_handle = NULL;
    }
}

void pwr_set_voltage_scale(pwr_handle_t handle, pwr_voltage_scale_t scale) {
    if (handle && s_is_handle_initialized) {
        handle->port_api->set_voltage_scale(handle, scale);
    }
}

void pwr_enter_stop_mode(pwr_handle_t handle) {
    if (handle && s_is_handle_initialized) {
        handle->port_api->enter_stop_mode(handle);
    }
}

void pwr_enter_standby_mode(pwr_handle_t handle) {
    if (handle && s_is_handle_initialized) {
        handle->port_api->enter_standby_mode(handle);
    }
}

void pwr_disable_backup_domain_write_protect(pwr_handle_t handle) {
    if (handle && s_is_handle_initialized) {
        handle->port_api->disable_backup_domain_write_protect(handle);
    }
}

void pwr_enable_backup_domain_write_protect(pwr_handle_t handle) {
    if (handle && s_is_handle_initialized) {
        handle->port_api->enable_backup_domain_write_protect(handle);
    }
}
