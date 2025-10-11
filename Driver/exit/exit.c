/**
 * @file      exti.c
 * @brief     Hardware-agnostic implementation of the EXTI driver.
 */

#include "internal/exti_private.h"
#include <string.h>

// --- Static Data ---
static struct exti_handle_t s_handle_pool;
static bool s_is_handle_in_use = {false};

// Map of line numbers (0-15) to their active handles.
static exti_handle_t s_line_to_handle_map[3] = {NULL};

// --- Private Helper Functions ---
static exti_handle_t allocate_handle(void) {
    for (int i = 0; i < EXTI_MAX_HANDLES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            return &s_handle_pool[i];
        }
    }
    return NULL;
}

static void release_handle(exti_handle_t handle) {
    for (int i = 0; i < EXTI_MAX_HANDLES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            return;
        }
    }
}

/**
 * @brief Generic IRQ handler called by the port-specific ISRs.
 * @details This function looks up the handle associated with the triggered
 *          line and invokes the user's callback.
 */
static void exti_generic_handler(uint8_t line_num) {
    if (line_num < 16) {
        exti_handle_t handle = s_line_to_handle_map[line_num];
        if (handle && handle->config.callback) {
            handle->config.callback(line_num, handle->config.user_data);
        }
    }
}

// --- Public API Function Implementations ---

exti_handle_t exti_init(uint8_t port_num, uint8_t pin_num, const exti_config_t* config) {
    if (config == NULL |

| pin_num > 15) {
        return NULL;
    }

    // Ensure this line isn't already in use
    if (s_line_to_handle_map[pin_num]!= NULL) {
        return NULL;
    }

    exti_handle_t handle = allocate_handle();
    if (handle == NULL) {
        return NULL;
    }

    // Use C tricks to write to const members during initialization
    *(uint8_t*)&handle->pin_num = pin_num;
    memcpy((void*)&handle->config, config, sizeof(exti_config_t));
    *(const exti_port_interface_t**)&handle->port_api = exti_port_get_api();

    if (handle->port_api == NULL) {
        release_handle(handle);
        return NULL;
    }

    s_line_to_handle_map[pin_num] = handle;

    handle->port_api->enable_syscfg_clock();
    handle->port_api->configure_interrupt(port_num, pin_num, config->trigger);
    handle->port_api->enable_irq(pin_num, exti_generic_handler);

    return handle;
}

void exti_deinit(exti_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        exti_handle_t handle = *p_handle;
        handle->port_api->disable_irq(handle->pin_num);
        s_line_to_handle_map[handle->pin_num] = NULL;
        release_handle(handle);
        *p_handle = NULL;
    }
}
