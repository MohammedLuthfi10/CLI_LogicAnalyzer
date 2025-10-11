/**
 * @file      pwr_port.h
 * @brief     Defines the abstract porting interface for the PWR driver.
 */

#ifndef PWR_PORT_H
#define PWR_PORT_H

#include "pwr.h"

struct pwr_handle_t;

typedef struct {
    void (*enable_clock)(void);
    void (*set_voltage_scale)(struct pwr_handle_t* handle, pwr_voltage_scale_t scale);
    void (*enter_stop_mode)(struct pwr_handle_t* handle);
    void (*enter_standby_mode)(struct pwr_handle_t* handle);
    void (*disable_backup_domain_write_protect)(struct pwr_handle_t* handle);
    void (*enable_backup_domain_write_protect)(struct pwr_handle_t* handle);
} pwr_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const pwr_port_interface_t* pwr_port_get_api(void);
void* pwr_port_get_base_addr(void);

#endif // PWR_PORT_H
