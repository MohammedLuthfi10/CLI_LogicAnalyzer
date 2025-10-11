/**
 * @file      timer_port.h
 * @brief     Defines the abstract porting interface for the Timer driver.
 */

#ifndef TIMER_PORT_H
#define TIMER_PORT_H

struct timer_handle_t;

typedef struct {
    void (*enable_clock)(uint8_t instance_num);
    void (*configure_core)(struct timer_handle_t* handle);
    void (*start)(struct timer_handle_t* handle);
    void (*stop)(struct timer_handle_t* handle);
    uint32_t (*get_counter)(struct timer_handle_t* handle);
    void (*enable_update_irq)(struct timer_handle_t* handle);
    void (*disable_update_irq)(struct timer_handle_t* handle);
    bool (*is_update_irq_flag_set)(struct timer_handle_t* handle);
    void (*clear_update_irq_flag)(struct timer_handle_t* handle);
} timer_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const timer_port_interface_t* timer_port_get_api(void);
void* timer_port_get_base_addr(uint8_t instance_num);

#endif // TIMER_PORT_H
