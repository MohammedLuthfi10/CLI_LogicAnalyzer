/**
 * @file      rtc.c
 * @brief     Hardware-agnostic implementation of the RTC driver.
 */

#include "internal/rtc_private.h"
#include <string.h>

// --- Static Data ---
static struct rtc_handle_t s_handle;
static bool s_is_handle_initialized = false;

// --- Public API Function Implementations ---

rtc_handle_t rtc_init(const rtc_config_t* config) {
    if (config == NULL |

| s_is_handle_initialized) {
        return NULL;
    }

    s_handle.port_api = rtc_port_get_api();
    s_handle.port_hw_instance = rtc_port_get_base_addr();

    if (s_handle.port_api == NULL |

| s_handle.port_hw_instance == NULL) {
        return NULL;
    }

    s_handle.port_api->enable_clock();
    if (s_handle.port_api->configure_prescalers(&s_handle, config)!= 0) {
        return NULL; // Failed to enter init mode
    }

    s_is_handle_initialized = true;
    return &s_handle;
}

void rtc_deinit(rtc_handle_t* p_handle) {
    if (p_handle!= NULL && *p_handle!= NULL) {
        s_is_handle_initialized = false;
        *p_handle = NULL;
    }
}

int rtc_set_time(rtc_handle_t handle, const rtc_time_t* time) {
    if (handle && s_is_handle_initialized && time) {
        return handle->port_api->set_time(handle, time);
    }
    return -1;
}

int rtc_get_time(rtc_handle_t handle, rtc_time_t* time) {
    if (handle && s_is_handle_initialized && time) {
        return handle->port_api->get_time(handle, time);
    }
    return -1;
}

int rtc_set_date(rtc_handle_t handle, const rtc_date_t* date) {
    if (handle && s_is_handle_initialized && date) {
        return handle->port_api->set_date(handle, date);
    }
    return -1;
}

int rtc_get_date(rtc_handle_t handle, rtc_date_t* date) {
    if (handle && s_is_handle_initialized && date) {
        return handle->port_api->get_date(handle, date);
    }
    return -1;
}
