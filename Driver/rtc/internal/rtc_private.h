/**
 * @file      rtc_private.h
 * @brief     Private internal definitions for the RTC driver.
 */

#ifndef RTC_PRIVATE_H
#define RTC_PRIVATE_H

#include "rtc.h"
#include "port/rtc_port.h"

/**
 * @brief The complete driver handle structure.
 */
struct rtc_handle_t {
    const rtc_port_interface_t* port_api;
    void* port_hw_instance;
};

#endif // RTC_PRIVATE_H
