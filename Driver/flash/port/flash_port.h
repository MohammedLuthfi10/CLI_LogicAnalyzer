/**
 * @file      flash_port.h
 * @brief     Defines the abstract porting interface for the FLASH driver.
 */

#ifndef FLASH_PORT_H
#define FLASH_PORT_H

struct flash_handle_t;

typedef struct {
    int (*set_wait_states)(struct flash_handle_t* handle, uint32_t system_clock_hz);
    int (*erase_sector)(struct flash_handle_t* handle, uint8_t sector_index);
    int (*program)(struct flash_handle_t* handle, uint32_t address, const uint8_t* data, size_t len);
} flash_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const flash_port_interface_t* flash_port_get_api(void);
void* flash_port_get_base_addr(void);

#endif // FLASH_PORT_H
