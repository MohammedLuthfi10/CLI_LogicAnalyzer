/**
 * @file      adc.c
 * @brief     Hardware-agnostic implementation of the ADC driver.
 */

#include "internal/adc_private.h"
#include <string.h>

// --- Static Data ---
static struct adc_handle_t s_handle_pool;
static bool s_is_handle_in_use = {false};

// --- Private Helper Functions ---
static struct adc_handle_t* allocate_handle(void) {
    for (int i = 0; i < ADC_MAX_INSTANCES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            memset(&s_handle_pool[i], 0, sizeof(struct adc_handle_t));
            return &s_handle_pool[i];
        }
    }
    return NULL;
}

static void release_handle(adc_handle_t handle) {
    for (int i = 0; i < ADC_MAX_INSTANCES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            return;
        }
    }
}

// --- Public API Function Implementations ---

adc_handle_t adc_init(uint8_t instance_num, const adc_config_t* config) {
    if (config == NULL) {
        return NULL;
    }

    struct adc_handle_t* handle = allocate_handle();
    if (handle == NULL) {
        return NULL;
    }

    *(const adc_port_interface_t**)&handle->port_api = adc_port_get_api();
    *(void**)&handle->port_hw_instance = adc_port_get_base_addr(instance_num);

    if (handle->port_api == NULL |

| handle->port_hw_instance == NULL) {
        release_handle(handle);
        return NULL;
    }

    memcpy((void*)&handle->config, config, sizeof(adc_config_t));

    handle->port_api->enable_clock(instance_num);
    handle->port_api->configure_core(handle);

    handle->context.is_initialized = true;
    return handle;
}

void adc_deinit(adc_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        adc_handle_t handle = *p_handle;
        if (handle->context.is_initialized) {
            handle->port_api->power_off(handle);
        }
        release_handle(handle);
        *p_handle = NULL;
    }
}

uint16_t adc_read_blocking(adc_handle_t handle, uint8_t channel) {
    if (handle && handle->context.is_initialized) {
        return handle->port_api->read_single_channel(handle, channel);
    }
    return 0;
}
