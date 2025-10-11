/**
 * @file      uart.c
 * @brief     Hardware-agnostic implementation of the UART driver.
 */

#include "internal/uart_private.h"
#include <string.h> // For memcpy

// --- Static Data ---

// A static pool of handles. This avoids the need for dynamic memory allocation.
static struct uart_handle_t s_handle_pool[UART_MAX_INSTANCES];
static bool s_is_handle_in_use[UART_MAX_INSTANCES] = {false};

// --- Private Helper Functions ---

/**
 * @brief Allocates a handle from the static pool.
 * @return Pointer to an available handle, or NULL if all are in use.
 */
static struct uart_handle_t* allocate_handle(void) {
    for (int i = 0; i < UART_MAX_INSTANCES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            // Zero out the handle to ensure a clean state
            memset(&s_handle_pool[i], 0, sizeof(struct uart_handle_t));
            return &s_handle_pool[i];
        }
    }
    return NULL; // No available handles
}

/**
 * @brief Releases a handle back to the static pool.
 * @param handle The handle to release.
 */
static void release_handle(uart_handle_t handle) {
    for (int i = 0; i < UART_MAX_INSTANCES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            return;
        }
    }
}

// --- Public API Function Implementations ---

uart_handle_t uart_init(uint8_t instance_num, const uart_config_t* config) {
    if (config == NULL) {
        return NULL;
    }

    struct uart_handle_t* handle = allocate_handle();
    if (handle == NULL) {
        return NULL; // No handles available
    }

    // Get the platform-specific implementation details
    handle->port_api = uart_port_get_api_for_instance(instance_num);
    handle->port_hw_instance = uart_port_get_base_addr_for_instance(instance_num);

    if (handle->port_api == NULL || handle->port_hw_instance == NULL) {
        release_handle(handle);
        return NULL; // Invalid instance number
    }

    // Copy the user's configuration into the handle's const member
    // This uses a C trick to write to a const member during initialization
    memcpy((void*)&handle->config, config, sizeof(uart_config_t));

    // Perform hardware initialization via the porting layer
    handle->port_api->enable_clock(handle);
    handle->port_api->init_pins(handle);
    handle->port_api->configure_core(handle);
    handle->port_api->enable(handle);

    handle->context.is_initialized = true;

    return handle;
}

void uart_deinit(uart_handle_t* p_handle) {
    if (p_handle == NULL || *p_handle == NULL) {
        return;
    }

    uart_handle_t handle = *p_handle;
    if (handle->context.is_initialized) {
        handle->port_api->disable(handle);
        handle->port_api->disable_clock(handle);
        handle->context.is_initialized = false;
    }

    release_handle(handle);
    *p_handle = NULL;
}

int uart_write_blocking(uart_handle_t handle, const uint8_t* p_data, size_t len) {
    if (handle == NULL || p_data == NULL ||!handle->context.is_initialized) {
        return -1; // Invalid arguments
    }

    for (size_t i = 0; i < len; ++i) {
        handle->port_api->write_byte_blocking(handle, p_data[i]);
    }

    return 0;
}

int uart_read_blocking(uart_handle_t handle, uint8_t* p_data, size_t len) {
    if (handle == NULL || p_data == NULL ||!handle->context.is_initialized) {
        return -1; // Invalid arguments
    }

    for (size_t i = 0; i < len; ++i) {
        p_data[i] = handle->port_api->read_byte_blocking(handle);
    }

    return 0;
}
