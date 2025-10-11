/**
 * @file      spi.c
 * @brief     Hardware-agnostic implementation of the SPI driver.
 */

#include "internal/spi_private.h"
#include <string.h>

// --- Static Data ---
static struct spi_handle_t s_handle_pool;
static bool s_is_handle_in_use = {false};

// --- Private Helper Functions ---
static struct spi_handle_t* allocate_handle(void) {
    for (int i = 0; i < SPI_MAX_INSTANCES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            memset(&s_handle_pool[i], 0, sizeof(struct spi_handle_t));
            return &s_handle_pool[i];
        }
    }
    return NULL;
}

static void release_handle(spi_handle_t handle) {
    for (int i = 0; i < SPI_MAX_INSTANCES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            return;
        }
    }
}

// --- Public API Function Implementations ---

spi_handle_t spi_init(uint8_t instance_num, const spi_config_t* config) {
    if (config == NULL) {
        return NULL;
    }

    struct spi_handle_t* handle = allocate_handle();
    if (handle == NULL) {
        return NULL;
    }

    handle->port_api = spi_port_get_api_for_instance(instance_num);
    handle->port_hw_instance = spi_port_get_base_addr_for_instance(instance_num);

    if (handle->port_api == NULL |

| handle->port_hw_instance == NULL) {
        release_handle(handle);
        return NULL;
    }

    memcpy((void*)&handle->config, config, sizeof(spi_config_t));

    handle->port_api->enable_clock(handle);
    handle->port_api->init_pins(handle);
    handle->port_api->configure_core(handle);
    handle->port_api->enable(handle);

    handle->context.is_initialized = true;
    return handle;
}

void spi_deinit(spi_handle_t* p_handle) {
    if (p_handle == NULL |

| *p_handle == NULL) {
        return;
    }
    spi_handle_t handle = *p_handle;
    if (handle->context.is_initialized) {
        handle->port_api->disable(handle);
        // Add disable_clock if implemented
    }
    release_handle(handle);
    *p_handle = NULL;
}

int spi_transfer_blocking(spi_handle_t handle, const uint8_t* p_tx_data, uint8_t* p_rx_data, size_t len) {
    if (handle == NULL ||!handle->context.is_initialized) {
        return -1; // Invalid arguments
    }

    const uint8_t DUMMY_BYTE = 0xFF;

    for (size_t i = 0; i < len; ++i) {
        uint8_t byte_to_send = (p_tx_data)? p_tx_data[i] : DUMMY_BYTE;
        uint8_t received_byte = handle->port_api->transfer_byte(handle, byte_to_send);
        if (p_rx_data) {
            p_rx_data[i] = received_byte;
        }
    }
    return 0;
}
