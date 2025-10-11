/**
 * @file      exti_port.h
 * @brief     Defines the abstract porting interface for the EXTI driver.
 */

#ifndef EXTI_PORT_H
#define EXTI_PORT_H

#include "exti.h"

typedef void (*exti_generic_handler_t)(uint8_t line_num);

typedef struct {
    void (*enable_syscfg_clock)(void);
    void (*configure_interrupt)(uint8_t port_num, uint8_t pin_num, exti_trigger_t trigger);
    void (*enable_irq)(uint8_t pin_num, exti_generic_handler_t handler);
    void (*disable_irq)(uint8_t pin_num);
} exti_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const exti_port_interface_t* exti_port_get_api(void);

#endif // EXTI_PORT_H
