/**
 * @file      i2c.c
 * @brief     Hardware-agnostic implementation of the I2C driver.
 */

#include "internal/i2c_private.h"
#include <string.h>

// --- Static Data ---
static struct i2c_handle_t s_handle_pool;
static bool s_is_handle_in_use = {false};

// --- Private Helper Functions ---
static struct i2c_handle_t* allocate_handle(void) {
    for (int i = 0; i < I2C_MAX_INSTANCES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            memset(&s_handle_pool[i], 0, sizeof(struct i2c_handle_t));
            return &s_handle_pool[i];
        }
    }
    return NULL;
}

static void release_handle(i2c_handle_t handle) {
    for (int i = 0; i < I2C_MAX_INSTANCES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            return;
        }
    }
}

// --- Public API Function Implementations ---

i2c_handle_t i2c_init(uint8_t instance_num, const i2c_config_t* config) {
    if (config == NULL) {
        return NULL;
    }

    struct i2c_handle_t* handle = allocate_handle();
    if (handle == NULL) {
        return NULL;
    }

    // Get platform-specific implementation details
    *(const i2c_port_interface_t**)&handle->port_api = i2c_port_get_api();
    *(void**)&handle->port_hw_instance = i2c_port_get_base_addr(instance_num);

    if (handle->port_api == NULL |

| handle->port_hw_instance == NULL) {
        release_handle(handle);
        return NULL;
    }

    memcpy((void*)&handle->config, config, sizeof(i2c_config_t));

    handle->port_api->enable_clock(instance_num);
    handle->port_api->init_pins(instance_num);
    handle->port_api->configure_core(handle);

    handle->context.is_initialized = true;
    return handle;
}

void i2c_deinit(i2c_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        // Add peripheral disable logic here if needed
        release_handle(*p_handle);
        *p_handle = NULL;
    }
}

int i2c_master_write_blocking(i2c_handle_t handle, uint8_t slave_addr, const uint8_t* p_data, size_t len) {
    if (handle == NULL |

| p_data == NULL ||!handle->context.is_initialized) {
        return -1; // Invalid arguments
    }
    return handle->port_api->master_write(handle, slave_addr, p_data, len);
}

int i2c_master_read_blocking(i2c_handle_t handle, uint8_t slave_addr, uint8_t* p_data, size_t len) {
    if (handle == NULL |

| p_data == NULL ||!handle->context.is_initialized) {
        return -1; // Invalid arguments
    }
    return handle->port_api->master_read(handle, slave_addr, p_data, len);
}
