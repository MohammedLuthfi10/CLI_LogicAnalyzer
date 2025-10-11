/**
 * @file      dac_port.h
 * @brief     Defines the abstract porting interface for the DAC driver.
 */

#ifndef DAC_PORT_H
#define DAC_PORT_H

struct dac_handle_t;

typedef struct {
    void (*enable_clock)(uint8_t instance_num);
    void (*init_pins)(uint8_t channel_num);
    void (*configure_channel)(struct dac_handle_t* handle);
    void (*write_value)(struct dac_handle_t* handle, uint16_t value);
} dac_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const dac_port_interface_t* dac_port_get_api(void);
void* dac_port_get_base_addr(uint8_t instance_num);

#endif // DAC_PORT_H
