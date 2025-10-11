/**
 * @file      dac.c
 * @brief     Hardware-agnostic implementation of the DAC driver.
 */

#include "internal/dac_private.h"
#include <string.h>

// --- Static Data ---
static struct dac_handle_t s_handle_pool;
static bool s_is_handle_in_use = {false};

// --- Private Helper Functions ---
static struct dac_handle_t* allocate_handle(void) {
    for (int i = 0; i < DAC_MAX_HANDLES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            return &s_handle_pool[i];
        }
    }
    return NULL;
}

static void release_handle(dac_handle_t handle) {
    for (int i = 0; i < DAC_MAX_HANDLES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            return;
        }
    }
}

// --- Public API Function Implementations ---

dac_handle_t dac_init(uint8_t instance_num, uint8_t channel_num, const dac_config_t* config) {
    if (config == NULL |

| (channel_num!= 1 && channel_num!= 2)) {
        return NULL;
    }

    dac_handle_t handle = allocate_handle();
    if (handle == NULL) {
        return NULL;
    }

    // Use C tricks to write to const members during initialization
    *(uint8_t*)&handle->channel_num = channel_num;
    memcpy((void*)&handle->config, config, sizeof(dac_config_t));

    *(const dac_port_interface_t**)&handle->port_api = dac_port_get_api();
    *(void**)&handle->port_hw_instance = dac_port_get_base_addr(instance_num);

    if (handle->port_api == NULL |

| handle->port_hw_instance == NULL) {
        release_handle(handle);
        return NULL;
    }

    handle->port_api->enable_clock(instance_num);
    handle->port_api->init_pins(channel_num);
    handle->port_api->configure_channel(handle);

    return handle;
}

void dac_deinit(dac_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        // Add channel disable logic if needed
        release_handle(*p_handle);
        *p_handle = NULL;
    }
}

int dac_write_value(dac_handle_t handle, uint16_t value) {
    if (handle) {
        handle->port_api->write_value(handle, value);
        return 0;
    }
    return -1;
}
