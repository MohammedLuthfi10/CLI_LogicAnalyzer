/**
 * @file      dma_port.h
 * @brief     Defines the abstract porting interface for the DMA driver.
 */

#ifndef DMA_PORT_H
#define DMA_PORT_H

struct dma_handle_t;

typedef struct {
    void (*enable_clock)(uint8_t dma_num);
    void (*configure_stream)(struct dma_handle_t* handle);
    void (*start_transfer)(struct dma_handle_t* handle, const void* src, void* dest, uint16_t count);
    void (*stop_transfer)(struct dma_handle_t* handle);
    void (*enable_interrupt)(struct dma_handle_t* handle, dma_interrupt_t interrupt);
    bool (*is_interrupt_flag_set)(struct dma_handle_t* handle, dma_interrupt_t interrupt);
    void (*clear_interrupt_flag)(struct dma_handle_t* handle, dma_interrupt_t interrupt);
} dma_port_interface_t;

/* --- Functions to be provided by the concrete port implementation --- */

const dma_port_interface_t* dma_port_get_api(void);
void* dma_port_get_base_addr(uint8_t dma_num);

#endif // DMA_PORT_H
