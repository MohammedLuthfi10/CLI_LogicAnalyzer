/**
 * @file      dma.c
 * @brief     Hardware-agnostic implementation of the DMA driver.
 */

#include "internal/dma_private.h"
#include <string.h>

// --- Static Data ---
static struct dma_handle_t s_handle_pool;
static bool s_is_handle_in_use = {false};

// --- Private Helper Functions ---
static struct dma_handle_t* allocate_handle(void) {
    for (int i = 0; i < DMA_MAX_HANDLES; ++i) {
        if (!s_is_handle_in_use[i]) {
            s_is_handle_in_use[i] = true;
            return &s_handle_pool[i];
        }
    }
    return NULL;
}

static void release_handle(dma_handle_t handle) {
    for (int i = 0; i < DMA_MAX_HANDLES; ++i) {
        if (handle == &s_handle_pool[i]) {
            s_is_handle_in_use[i] = false;
            return;
        }
    }
}

// --- Public API Function Implementations ---

dma_handle_t dma_init(uint8_t dma_num, uint8_t stream_num, const dma_config_t* config) {
    if (config == NULL |

| (dma_num!= 1 && dma_num!= 2) |
| stream_num > 7) {
        return NULL;
    }

    dma_handle_t handle = allocate_handle();
    if (handle == NULL) {
        return NULL;
    }

    // Use C tricks to write to const members during initialization
    *(uint8_t*)&handle->dma_num = dma_num;
    *(uint8_t*)&handle->stream_num = stream_num;
    memcpy((void*)&handle->config, config, sizeof(dma_config_t));

    *(const dma_port_interface_t**)&handle->port_api = dma_port_get_api();
    *(void**)&handle->port_controller_instance = dma_port_get_base_addr(dma_num);

    // Calculate stream base address
    dma_controller_reg_map_t* controller = (dma_controller_reg_map_t*)handle->port_controller_instance;
    *(void**)&handle->port_stream_instance = &controller->S[stream_num];

    if (handle->port_api == NULL |

| handle->port_controller_instance == NULL) {
        release_handle(handle);
        return NULL;
    }

    handle->port_api->enable_clock(dma_num);
    handle->port_api->configure_stream(handle);

    return handle;
}

void dma_deinit(dma_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        dma_stop_transfer(*p_handle);
        release_handle(*p_handle);
        *p_handle = NULL;
    }
}

void dma_start_transfer(dma_handle_t handle, const void* source_address, void* destination_address, uint16_t data_count) {
    if (handle) {
        handle->port_api->start_transfer(handle, source_address, destination_address, data_count);
    }
}

void dma_stop_transfer(dma_handle_t handle) {
    if (handle) {
        handle->port_api->stop_transfer(handle);
    }
}

void dma_enable_interrupt(dma_handle_t handle, dma_interrupt_t interrupt) {
    if (handle) {
        handle->port_api->enable_interrupt(handle, interrupt);
    }
}

bool dma_is_interrupt_flag_set(dma_handle_t handle, dma_interrupt_t interrupt) {
    if (handle) {
        return handle->port_api->is_interrupt_flag_set(handle, interrupt);
    }
    return false;
}

void dma_clear_interrupt_flag(dma_handle_t handle, dma_interrupt_t interrupt) {
    if (handle) {
        handle->port_api->clear_interrupt_flag(handle, interrupt);
    }
}
