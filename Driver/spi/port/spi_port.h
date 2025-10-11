/**
 * @file      spi_port.h
 * @brief     Defines the abstract porting interface for the SPI driver.
 */

#ifndef SPI_PORT_H
#define SPI_PORT_H

struct spi_handle_t;

/**
 * @brief A structure of function pointers that defines the hardware-dependent
 *        operations required by the SPI driver.
 */
typedef struct {
    void (*enable_clock)(struct spi_handle_t* handle);
    void (*init_pins)(struct spi_handle_t* handle);
    void (*configure_core)(struct spi_handle_t* handle);
    uint8_t (*transfer_byte)(struct spi_handle_t* handle, uint8_t tx_byte);
    void (*enable)(struct spi_handle_t* handle);
    void (*disable)(struct spi_handle_t* handle);
} spi_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const spi_port_interface_t* spi_port_get_api_for_instance(uint8_t instance_num);
void* spi_port_get_base_addr_for_instance(uint8_t instance_num);

#endif // SPI_PORT_H
