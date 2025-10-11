/**
 * @file      i2c_port.h
 * @brief     Defines the abstract porting interface for the I2C driver.
 */

#ifndef I2C_PORT_H
#define I2C_PORT_H

struct i2c_handle_t;

/**
 * @brief A structure of function pointers that defines the hardware-dependent
 *        operations required by the I2C driver.
 */
typedef struct {
    void (*enable_clock)(uint8_t instance_num);
    void (*init_pins)(uint8_t instance_num);
    void (*configure_core)(struct i2c_handle_t* handle);
    int (*master_write)(struct i2c_handle_t* handle, uint8_t addr, const uint8_t* data, size_t len);
    int (*master_read)(struct i2c_handle_t* handle, uint8_t addr, uint8_t* data, size_t len);
} i2c_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const i2c_port_interface_t* i2c_port_get_api(void);
void* i2c_port_get_base_addr(uint8_t instance_num);

#endif // I2C_PORT_H
