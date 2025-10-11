/**
 * @file      dma_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/dma_private.h"
#include "internal/dma_reg.h"

// Placeholder base addresses
#define AHB1PERIPH_BASE       0x40020000UL
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000UL)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400UL)

// --- Private Helper Functions for STM32F4 ---

// Offsets for interrupt flags within the LISR/HISR registers
static const uint8_t flag_offsets = {0, 6, 16, 22};
#define DMA_FLAG_TCIF (1 << 5)
#define DMA_FLAG_HTIF (1 << 4)
#define DMA_FLAG_TEIF (1 << 3)

// --- Port Implementation ---

static void stm32f4_enable_clock(uint8_t dma_num) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., if (dma_num == 1) rcc_enable_peripheral_clock(PERIPH_ID_DMA1);
}

static void stm32f4_configure_stream(struct dma_handle_t* handle) {
    dma_stream_reg_map_t* stream_regs = (dma_stream_reg_map_t*)handle->port_stream_instance;
    const dma_config_t* config = &handle->config;

    // Ensure stream is disabled before configuring
    stream_regs->CR &= ~DMA_SxCR_EN_Msk;
    while (stream_regs->CR & DMA_SxCR_EN_Msk);

    uint32_t cr = 0;
    cr |= (config->channel << DMA_SxCR_CHSEL_Pos);
    cr |= (config->priority << DMA_SxCR_PL_Pos);
    cr |= (config->direction << DMA_SxCR_DIR_Pos);
    cr |= (config->peripheral_data_size << DMA_SxCR_PSIZE_Pos);
    cr |= (config->memory_data_size << DMA_SxCR_MSIZE_Pos);
    if (config->peripheral_increment) { cr |= DMA_SxCR_PINC_Msk; }
    if (config->memory_increment) { cr |= DMA_SxCR_MINC_Msk; }
    if (config->circular_mode) { cr |= DMA_SxCR_CIRC_Msk; }

    stream_regs->CR = cr;
}

static void stm32f4_start_transfer(struct dma_handle_t* handle, const void* src, void* dest, uint16_t count) {
    dma_stream_reg_map_t* stream_regs = (dma_stream_reg_map_t*)handle->port_stream_instance;

    // Ensure stream is disabled
    stream_regs->CR &= ~DMA_SxCR_EN_Msk;
    while (stream_regs->CR & DMA_SxCR_EN_Msk);

    stream_regs->NDTR = count;

    if (handle->config.direction == DMA_DIRECTION_MEMORY_TO_PERIPHERAL) {
        stream_regs->PAR = (uint32_t)dest;
        stream_regs->M0AR = (uint32_t)src;
    } else {
        stream_regs->PAR = (uint32_t)src;
        stream_regs->M0AR = (uint32_t)dest;
    }

    // Clear all flags for this stream before starting
    dma_port_get_api()->clear_interrupt_flag(handle, DMA_INTERRUPT_TRANSFER_COMPLETE);
    dma_port_get_api()->clear_interrupt_flag(handle, DMA_INTERRUPT_HALF_TRANSFER);
    dma_port_get_api()->clear_interrupt_flag(handle, DMA_INTERRUPT_TRANSFER_ERROR);

    stream_regs->CR |= DMA_SxCR_EN_Msk;
}

static void stm32f4_stop_transfer(struct dma_handle_t* handle) {
    dma_stream_reg_map_t* stream_regs = (dma_stream_reg_map_t*)handle->port_stream_instance;
    stream_regs->CR &= ~DMA_SxCR_EN_Msk;
}

static void stm32f4_enable_interrupt(struct dma_handle_t* handle, dma_interrupt_t interrupt) {
    dma_stream_reg_map_t* stream_regs = (dma_stream_reg_map_t*)handle->port_stream_instance;
    switch (interrupt) {
        case DMA_INTERRUPT_TRANSFER_COMPLETE: stream_regs->CR |= DMA_SxCR_TCIE_Msk; break;
        case DMA_INTERRUPT_HALF_TRANSFER:     stream_regs->CR |= DMA_SxCR_HTIE_Msk; break;
        case DMA_INTERRUPT_TRANSFER_ERROR:    stream_regs->CR |= DMA_SxCR_TEIE_Msk; break;
    }
}

static bool stm32f4_is_interrupt_flag_set(struct dma_handle_t* handle, dma_interrupt_t interrupt) {
    dma_controller_reg_map_t* dma_regs = (dma_controller_reg_map_t*)handle->port_controller_instance;
    uint8_t stream = handle->stream_num;
    uint32_t flag = 0;

    switch (interrupt) {
        case DMA_INTERRUPT_TRANSFER_COMPLETE: flag = DMA_FLAG_TCIF; break;
        case DMA_INTERRUPT_HALF_TRANSFER:     flag = DMA_FLAG_HTIF; break;
        case DMA_INTERRUPT_TRANSFER_ERROR:    flag = DMA_FLAG_TEIF; break;
    }

    uint32_t offset = flag_offsets[stream % 4];
    if (stream < 4) {
        return (dma_regs->LISR & (flag << offset))!= 0;
    } else {
        return (dma_regs->HISR & (flag << offset))!= 0;
    }
}

static void stm32f4_clear_interrupt_flag(struct dma_handle_t* handle, dma_interrupt_t interrupt) {
    dma_controller_reg_map_t* dma_regs = (dma_controller_reg_map_t*)handle->port_controller_instance;
    uint8_t stream = handle->stream_num;
    uint32_t flag = 0;

    switch (interrupt) {
        case DMA_INTERRUPT_TRANSFER_COMPLETE: flag = DMA_FLAG_TCIF; break;
        case DMA_INTERRUPT_HALF_TRANSFER:     flag = DMA_FLAG_HTIF; break;
        case DMA_INTERRUPT_TRANSFER_ERROR:    flag = DMA_FLAG_TEIF; break;
    }

    uint32_t offset = flag_offsets[stream % 4];
    if (stream < 4) {
        dma_regs->LIFCR = (flag << offset);
    } else {
        dma_regs->HIFCR = (flag << offset);
    }
}

// --- The concrete port interface for STM32F4 ---
static const dma_port_interface_t stm32f4_port_api = {
   .enable_clock = stm32f4_enable_clock,
   .configure_stream = stm32f4_configure_stream,
   .start_transfer = stm32f4_start_transfer,
   .stop_transfer = stm32f4_stop_transfer,
   .enable_interrupt = stm32f4_enable_interrupt,
   .is_interrupt_flag_set = stm32f4_is_interrupt_flag_set,
   .clear_interrupt_flag = stm32f4_clear_interrupt_flag,
};

// --- Public functions provided by the port ---
const dma_port_interface_t* dma_port_get_api(void) {
    return &stm32f4_port_api;
}

void* dma_port_get_base_addr(uint8_t dma_num) {
    if (dma_num == 1) return (void*)DMA1_BASE;
    if (dma_num == 2) return (void*)DMA2_BASE;
    return NULL;
}
