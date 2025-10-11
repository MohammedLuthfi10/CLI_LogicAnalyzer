/**
 * @file      gpio.c
 * @brief     Hardware-agnostic implementation of the GPIO driver.
 */

#include "internal/gpio_private.h"
#include <string.h>

// --- Static Data ---
static struct gpio_handle_t s_handle_pool;
static bool s_is_handle_in_use = {false};

// --- Private Helper Functions ---
static struct gpio_handle_t* allocate_handle(void) {
    for (int i = 0; i < GPIO_MAX_HANDLES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            return &s_handle_pool[i];
        }
    }
    return NULL;
}

static void release_handle(gpio_handle_t handle) {
    for (int i = 0; i < GPIO_MAX_HANDLES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            // Optional: Zero out the handle
            memset(handle, 0, sizeof(struct gpio_handle_t));
            return;
        }
    }
}

// --- Public API Function Implementations ---

gpio_handle_t gpio_init(uint8_t port_num, uint16_t pin_mask, const gpio_config_t* config) {
    if (config == NULL |

| pin_mask == 0) {
        return NULL;
    }

    struct gpio_handle_t* handle = allocate_handle();
    if (handle == NULL) {
        return NULL;
    }

    // Use C tricks to write to const members during initialization
    *(const gpio_port_interface_t**)&handle->port_api = gpio_port_get_api();
    *(const void**)&handle->port_hw_instance = gpio_port_get_base_addr(port_num);
    *(uint16_t*)&handle->pin_mask = pin_mask;
    memcpy((void*)&handle->config, config, sizeof(gpio_config_t));

    if (handle->port_hw_instance == NULL) {
        release_handle(handle);
        return NULL;
    }

    handle->port_api->enable_clock(port_num);
    handle->port_api->configure_pins(handle);

    return handle;
}

void gpio_deinit(gpio_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        release_handle(*p_handle);
        *p_handle = NULL;
    }
}

void gpio_set(gpio_handle_t handle) {
    if (handle) {
        handle->port_api->set_pins((void*)handle->port_hw_instance, handle->pin_mask);
    }
}

void gpio_clear(gpio_handle_t handle) {
    if (handle) {
        handle->port_api->clear_pins((void*)handle->port_hw_instance, handle->pin_mask);
    }
}

void gpio_toggle(gpio_handle_t handle) {
    if (handle) {
        handle->port_api->toggle_pins((void*)handle->port_hw_instance, handle->pin_mask);
    }
}

bool gpio_read(gpio_handle_t handle) {
    if (handle && handle->pin_mask!= 0) {
        return (handle->port_api->read_pins((void*)handle->port_hw_instance) & handle->pin_mask)!= 0;
    }
    return false;
}
