/**
 * @file      iwdg_port.h
 * @brief     Defines the abstract porting interface for the IWDG driver.
 */

#ifndef IWDG_PORT_H
#define IWDG_PORT_H

#include "iwdg.h"

struct iwdg_handle_t;

typedef struct {
    void (*start_and_configure)(struct iwdg_handle_t* handle, uint32_t timeout_ms);
    void (*feed)(struct iwdg_handle_t* handle);
} iwdg_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const iwdg_port_interface_t* iwdg_port_get_api(void);
void* iwdg_port_get_base_addr(void);

#endif // IWDG_PORT_H
