/**
 * @file      adc_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/adc_private.h"
#include "internal/adc_reg.h"

// Placeholder base addresses
#define APB2PERIPH_BASE       0x40010000UL
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000UL)
#define ADC_COMMON_BASE       (ADC1_BASE + 0x300UL)

// --- Private function implementations for STM32F4 ---

static void stm32f4_enable_clock(uint8_t instance_num) {
    // Placeholder: A real implementation would use the RCC driver.
    // e.g., rcc_enable_peripheral_clock(PERIPH_ID_ADC1);
}

static void stm32f4_power_on(struct adc_handle_t* handle) {
    ((adc_reg_map_t*)handle->port_hw_instance)->CR2 |= ADC_CR2_ADON_Msk;
}

static void stm32f4_power_off(struct adc_handle_t* handle) {
    ((adc_reg_map_t*)handle->port_hw_instance)->CR2 &= ~ADC_CR2_ADON_Msk;
}

static void stm32f4_configure_core(struct adc_handle_t* handle) {
    adc_reg_map_t* adc_regs = (adc_reg_map_t*)handle->port_hw_instance;
    adc_common_reg_map_t* adc_common_regs = (adc_common_reg_map_t*)adc_port_get_common_base_addr();
    const adc_config_t* config = &handle->config;

    // Set common prescaler (e.g., PCLK2/4)
    adc_common_regs->CCR = (1 << ADC_CCR_ADCPRE_Pos);

    // Set resolution
    adc_regs->CR1 &= ~ADC_CR1_RES_Msk;
    adc_regs->CR1 |= (config->resolution << ADC_CR1_RES_Pos);

    // Set single conversion, right-aligned, disable scan mode
    adc_regs->CR1 &= ~ADC_CR1_SCAN_Msk;
    adc_regs->CR2 &= ~(ADC_CR2_CONT_Msk | ADC_CR2_ALIGN_Msk);

    // Power on the ADC
    stm32f4_power_on(handle);
}

static uint16_t stm32f4_read_single_channel(struct adc_handle_t* handle, uint8_t channel) {
    adc_reg_map_t* adc_regs = (adc_reg_map_t*)handle->port_hw_instance;
    const adc_config_t* config = &handle->config;

    // 1. Set sample time for the channel
    if (channel < 10) {
        adc_regs->SMPR2 &= ~(0b111 << (channel * 3));
        adc_regs->SMPR2 |= (config->default_sample_time << (channel * 3));
    } else {
        adc_regs->SMPR1 &= ~(0b111 << ((channel - 10) * 3));
        adc_regs->SMPR1 |= (config->default_sample_time << ((channel - 10) * 3));
    }

    // 2. Set regular sequence to 1 conversion of the specified channel
    adc_regs->SQR1 &= ~ADC_SQR1_L_Msk; // Length = 1 conversion
    adc_regs->SQR3 = channel;

    // 3. Start conversion
    adc_regs->CR2 |= ADC_CR2_SWSTART_Msk;

    // 4. Wait for End of Conversion
    while (!(adc_regs->SR & ADC_SR_EOC_Msk));

    // 5. Read and return data (reading DR also clears the EOC flag)
    return (uint16_t)adc_regs->DR;
}

// --- The concrete port interface for STM32F4 ---
static const adc_port_interface_t stm32f4_port_api = {
  .enable_clock = stm32f4_enable_clock,
  .configure_core = stm32f4_configure_core,
  .power_on = stm32f4_power_on,
  .power_off = stm32f4_power_off,
  .read_single_channel = stm32f4_read_single_channel,
};

// --- Public functions provided by the port ---
const adc_port_interface_t* adc_port_get_api(void) {
    return &stm32f4_port_api;
}

void* adc_port_get_base_addr(uint8_t instance_num) {
    if (instance_num == 1) {
        return (void*)ADC1_BASE;
    }
    // Add ADC2, ADC3 as needed
    return NULL;
}

void* adc_port_get_common_base_addr(void) {
    return (void*)ADC_COMMON_BASE;
}
