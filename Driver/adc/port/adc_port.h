/**
 * @file      adc_port.h
 * @brief     Defines the abstract porting interface for the ADC driver.
 */

#ifndef ADC_PORT_H
#define ADC_PORT_H

struct adc_handle_t;

typedef struct {
    void (*enable_clock)(uint8_t instance_num);
    void (*configure_core)(struct adc_handle_t* handle);
    void (*power_on)(struct adc_handle_t* handle);
    void (*power_off)(struct adc_handle_t* handle);
    uint16_t (*read_single_channel)(struct adc_handle_t* handle, uint8_t channel);
} adc_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const adc_port_interface_t* adc_port_get_api(void);
void* adc_port_get_base_addr(uint8_t instance_num);
void* adc_port_get_common_base_addr(void);

#endif // ADC_PORT_H
