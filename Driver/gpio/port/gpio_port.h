/**
 * @file      gpio_port.h
 * @brief     Defines the abstract porting interface for the GPIO driver.
 */

#ifndef GPIO_PORT_H
#define GPIO_PORT_H

struct gpio_handle_t;

/**
 * @brief A structure of function pointers that defines the hardware-dependent
 *        operations required by the GPIO driver.
 */
typedef struct {
    void (*enable_clock)(uint8_t port_num);
    void (*configure_pins)(struct gpio_handle_t* handle);
    void (*set_pins)(void* port_hw_instance, uint16_t pin_mask);
    void (*clear_pins)(void* port_hw_instance, uint16_t pin_mask);
    void (*toggle_pins)(void* port_hw_instance, uint16_t pin_mask);
    uint16_t (*read_pins)(void* port_hw_instance);
} gpio_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const gpio_port_interface_t* gpio_port_get_api(void);
void* gpio_port_get_base_addr(uint8_t port_num);

#endif // GPIO_PORT_H
