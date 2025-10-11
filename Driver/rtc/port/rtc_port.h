/**
 * @file      rtc_port.h
 * @brief     Defines the abstract porting interface for the RTC driver.
 */

#ifndef RTC_PORT_H
#define RTC_PORT_H

#include "rtc.h"

struct rtc_handle_t;

typedef struct {
    void (*enable_clock)(void);
    int (*configure_prescalers)(struct rtc_handle_t* handle, const rtc_config_t* config);
    int (*set_time)(struct rtc_handle_t* handle, const rtc_time_t* time);
    int (*get_time)(struct rtc_handle_t* handle, rtc_time_t* time);
    int (*set_date)(struct rtc_handle_t* handle, const rtc_date_t* date);
    int (*get_date)(struct rtc_handle_t* handle, rtc_date_t* date);
} rtc_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const rtc_port_interface_t* rtc_port_get_api(void);
void* rtc_port_get_base_addr(void);

#endif // RTC_PORT_H
