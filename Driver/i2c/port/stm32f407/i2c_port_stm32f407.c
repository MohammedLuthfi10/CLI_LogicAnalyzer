/**
 * @file      i2c_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/i2c_private.h"
#include "internal/i2c_reg.h"

// Placeholder base addresses
#define APB1PERIPH_BASE       0x40000000UL
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800UL)

// --- Private function implementations for STM32F4 ---

static void stm32f4_enable_clock(uint8_t instance_num) {
    // Placeholder: A real implementation would use the RCC driver.
}

static void stm32f4_init_pins(uint8_t instance_num) {
    // Placeholder: A real implementation would use a GPIO driver.
}

static void stm32f4_configure_core(struct i2c_handle_t* handle) {
    i2c_reg_map_t* i2c_regs = (i2c_reg_map_t*)handle->port_hw_instance;
    const i2c_config_t* config = &handle->config;

    // 1. Disable the peripheral
    i2c_regs->CR1 &= ~I2C_CR1_PE_Msk;

    // 2. Set peripheral clock frequency
    uint32_t pclk1_mhz = config->peripheral_clock_hz / 1000000;
    i2c_regs->CR2 &= ~I2C_CR2_FREQ_Msk;
    i2c_regs->CR2 |= (pclk1_mhz << I2C_CR2_FREQ_Pos);

    // 3. Configure CCR and TRISE
    if (config->speed == I2C_SPEED_STANDARD_100KHZ) {
        // Standard Mode
        i2c_regs->CCR &= ~I2C_CCR_FS_Msk;
        uint16_t ccr_val = (config->peripheral_clock_hz / (100000 * 2));
        i2c_regs->CCR = (ccr_val & I2C_CCR_CCR_Msk);
        i2c_regs->TRISE = pclk1_mhz + 1;
    } else {
        // Fast Mode
        i2c_regs->CCR |= I2C_CCR_FS_Msk;
        // Assuming Tlow/Thigh = 2
        uint16_t ccr_val = (config->peripheral_clock_hz / (400000 * 3));
        i2c_regs->CCR = (ccr_val & I2C_CCR_CCR_Msk);
        i2c_regs->TRISE = ((pclk1_mhz * 300) / 1000) + 1;
    }

    // 4. Enable the peripheral
    i2c_regs->CR1 |= I2C_CR1_PE_Msk;
}

static int stm32f4_master_write(struct i2c_handle_t* handle, uint8_t addr, const uint8_t* data, size_t len) {
    i2c_reg_map_t* i2c_regs = (i2c_reg_map_t*)handle->port_hw_instance;

    // 1. Send START condition
    i2c_regs->CR1 |= I2C_CR1_START_Msk;
    while (!(i2c_regs->SR1 & I2C_SR1_SB_Msk));

    // 2. Send slave address with write bit
    i2c_regs->DR = (addr << 1) | 0;
    while (!(i2c_regs->SR1 & I2C_SR1_ADDR_Msk));
    (void)i2c_regs->SR2; // Clear ADDR flag

    // 3. Send data bytes
    for (size_t i = 0; i < len; ++i) {
        i2c_regs->DR = data[i];
        while (!(i2c_regs->SR1 & I2C_SR1_TxE_Msk));
    }

    // 4. Wait for transfer to complete and send STOP
    while (!(i2c_regs->SR1 & I2C_SR1_BTF_Msk));
    i2c_regs->CR1 |= I2C_CR1_STOP_Msk;

    return 0; // Basic success, add error checks for NACK etc.
}

static int stm32f4_master_read(struct i2c_handle_t* handle, uint8_t addr, uint8_t* data, size_t len) {
    i2c_reg_map_t* i2c_regs = (i2c_reg_map_t*)handle->port_hw_instance;

    // 1. Enable ACK
    i2c_regs->CR1 |= I2C_CR1_ACK_Msk;

    // 2. Send START condition
    i2c_regs->CR1 |= I2C_CR1_START_Msk;
    while (!(i2c_regs->SR1 & I2C_SR1_SB_Msk));

    // 3. Send slave address with read bit
    i2c_regs->DR = (addr << 1) | 1;
    while (!(i2c_regs->SR1 & I2C_SR1_ADDR_Msk));
    (void)i2c_regs->SR2; // Clear ADDR flag

    // 4. Read data bytes
    for (size_t i = 0; i < len; ++i) {
        if (i == len - 1) {
            // Last byte: disable ACK before reading
            i2c_regs->CR1 &= ~I2C_CR1_ACK_Msk;
        }
        while (!(i2c_regs->SR1 & I2C_SR1_RxNE_Msk));
        data[i] = (uint8_t)i2c_regs->DR;
    }

    // 5. Send STOP condition
    i2c_regs->CR1 |= I2C_CR1_STOP_Msk;

    return 0;
}

// --- The concrete port interface for STM32F4 ---
static const i2c_port_interface_t stm32f4_port_api = {
  .enable_clock = stm32f4_enable_clock,
  .init_pins = stm32f4_init_pins,
  .configure_core = stm32f4_configure_core,
  .master_write = stm32f4_master_write,
  .master_read = stm32f4_master_read,
};

// --- Public functions provided by the port ---
const i2c_port_interface_t* i2c_port_get_api(void) {
    return &stm32f4_port_api;
}

void* i2c_port_get_base_addr(uint8_t instance_num) {
    switch (instance_num) {
        case 1: return (void*)I2C1_BASE;
        case 2: return (void*)I2C2_BASE;
        default: return NULL;
    }
}
