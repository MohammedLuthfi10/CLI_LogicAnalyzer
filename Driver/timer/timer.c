/**
 * @file      timer.c
 * @brief     Hardware-agnostic implementation of the Timer driver.
 */

#include "internal/timer_private.h"
#include <string.h>

// --- Static Data ---
static struct timer_handle_t s_handle_pool;
static bool s_is_handle_in_use = {false};

// --- Private Helper Functions ---
static struct timer_handle_t* allocate_handle(void) {
    for (int i = 0; i < TIMER_MAX_INSTANCES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            memset(&s_handle_pool[i], 0, sizeof(struct timer_handle_t));
            return &s_handle_pool[i];
        }
    }
    return NULL;
}

static void release_handle(timer_handle_t handle) {
    for (int i = 0; i < TIMER_MAX_INSTANCES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            return;
        }
    }
}

// --- Public API Function Implementations ---

timer_handle_t timer_init(uint8_t instance_num, const timer_config_t* config) {
    if (config == NULL) {
        return NULL;
    }

    struct timer_handle_t* handle = allocate_handle();
    if (handle == NULL) {
        return NULL;
    }

    *(const timer_port_interface_t**)&handle->port_api = timer_port_get_api();
    *(void**)&handle->port_hw_instance = timer_port_get_base_addr(instance_num);

    if (handle->port_api == NULL |

| handle->port_hw_instance == NULL) {
        release_handle(handle);
        return NULL;
    }

    memcpy((void*)&handle->config, config, sizeof(timer_config_t));

    handle->port_api->enable_clock(instance_num);
    handle->port_api->configure_core(handle);

    handle->context.is_initialized = true;
    return handle;
}

void timer_deinit(timer_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        timer_stop(*p_handle);
        // Add disable_clock if implemented
        release_handle(*p_handle);
        *p_handle = NULL;
    }
}

void timer_start(timer_handle_t handle) {
    if (handle && handle->context.is_initialized) {
        handle->port_api->start(handle);
    }
}

void timer_stop(timer_handle_t handle) {
    if (handle && handle->context.is_initialized) {
        handle->port_api->stop(handle);
    }
}

uint32_t timer_get_counter(timer_handle_t handle) {
    if (handle && handle->context.is_initialized) {
        return handle->port_api->get_counter(handle);
    }
    return 0;
}

void timer_enable_update_interrupt(timer_handle_t handle) {
    if (handle && handle->context.is_initialized) {
        handle->port_api->enable_update_irq(handle);
    }
}

void timer_disable_update_interrupt(timer_handle_t handle) {
    if (handle && handle->context.is_initialized) {
        handle->port_api->disable_update_irq(handle);
    }
}

bool timer_is_update_interrupt_flag_set(timer_handle_t handle) {
    if (handle && handle->context.is_initialized) {
        return handle->port_api->is_update_irq_flag_set(handle);
    }
    return false;
}

void timer_clear_update_interrupt_flag(timer_handle_t handle) {
    if (handle && handle->context.is_initialized) {
        handle->port_api->clear_update_irq_flag(handle);
    }
}
