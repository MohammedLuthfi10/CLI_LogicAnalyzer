/**
 * @file      rtc_port_stm32f4.c
 * @brief     Concrete porting layer implementation for the STM32F4xx series.
 */

#include "internal/rtc_private.h"
#include "internal/rtc_reg.h"

// Placeholder base address
#define RTC_BASE              0x40002800UL

// --- Private Helper Functions for STM32F4 ---

static uint8_t bcd_to_dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

static uint8_t dec_to_bcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

static int enter_init_mode(rtc_reg_map_t* rtc_regs) {
    rtc_regs->WPR = RTC_WPR_KEY1;
    rtc_regs->WPR = RTC_WPR_KEY2;
    rtc_regs->ISR |= RTC_ISR_INIT_Msk;
    for (volatile int i = 0; i < 10000; ++i) {
        if (rtc_regs->ISR & RTC_ISR_INITF_Msk) {
            return 0;
        }
    }
    return -1; // Timeout
}

static void exit_init_mode(rtc_reg_map_t* rtc_regs) {
    rtc_regs->ISR &= ~RTC_ISR_INIT_Msk;
    rtc_regs->WPR = 0xFF; // Re-lock
}

static void wait_for_sync(rtc_reg_map_t* rtc_regs) {
    rtc_regs->ISR &= ~RTC_ISR_RSF_Msk;
    while (!(rtc_regs->ISR & RTC_ISR_RSF_Msk));
}

// --- Port Implementation ---

static void stm32f4_enable_clock(void) {
    // Placeholder: A real implementation would use the RCC and PWR drivers
    // to enable the APB1 clock, PWR clock, and disable backup domain protection.
}

static int stm32f4_configure_prescalers(struct rtc_handle_t* handle, const rtc_config_t* config) {
    rtc_reg_map_t* rtc_regs = (rtc_reg_map_t*)handle->port_hw_instance;

    if (enter_init_mode(rtc_regs)!= 0) return -1;

    uint32_t prer = (config->async_prescaler << RTC_PRER_PREDIV_A_Pos) |
                    (config->sync_prescaler << RTC_PRER_PREDIV_S_Pos);
    rtc_regs->PRER = prer;

    exit_init_mode(rtc_regs);
    return 0;
}

static int stm32f4_set_time(struct rtc_handle_t* handle, const rtc_time_t* time) {
    rtc_reg_map_t* rtc_regs = (rtc_reg_map_t*)handle->port_hw_instance;
    if (enter_init_mode(rtc_regs)!= 0) return -1;

    uint32_t tr = (dec_to_bcd(time->hours) << 16) |
                  (dec_to_bcd(time->minutes) << 8) |
                  (dec_to_bcd(time->seconds));
    rtc_regs->TR = tr;

    exit_init_mode(rtc_regs);
    return 0;
}

static int stm32f4_get_time(struct rtc_handle_t* handle, rtc_time_t* time) {
    rtc_reg_map_t* rtc_regs = (rtc_reg_map_t*)handle->port_hw_instance;
    wait_for_sync(rtc_regs);
    uint32_t tr = rtc_regs->TR;

    time->seconds = bcd_to_dec((tr >> 0) & 0x7F);
    time->minutes = bcd_to_dec((tr >> 8) & 0x7F);
    time->hours = bcd_to_dec((tr >> 16) & 0x3F);
    return 0;
}

static int stm32f4_set_date(struct rtc_handle_t* handle, const rtc_date_t* date) {
    rtc_reg_map_t* rtc_regs = (rtc_reg_map_t*)handle->port_hw_instance;
    if (enter_init_mode(rtc_regs)!= 0) return -1;

    uint32_t dr = (dec_to_bcd(date->year) << 16) |
                  (date->weekday << 13) |
                  (dec_to_bcd(date->month) << 8) |
                  (dec_to_bcd(date->day));
    rtc_regs->DR = dr;

    exit_init_mode(rtc_regs);
    return 0;
}

static int stm32f4_get_date(struct rtc_handle_t* handle, rtc_date_t* date) {
    rtc_reg_map_t* rtc_regs = (rtc_reg_map_t*)handle->port_hw_instance;
    wait_for_sync(rtc_regs);
    uint32_t dr = rtc_regs->DR;

    date->day = bcd_to_dec((dr >> 0) & 0x3F);
    date->month = bcd_to_dec((dr >> 8) & 0x1F);
    date->weekday = (dr >> 13) & 0x07;
    date->year = bcd_to_dec((dr >> 16) & 0xFF);
    return 0;
}

// --- The concrete port interface for STM32F4 ---
static const rtc_port_interface_t stm32f4_port_api = {
.enable_clock = stm32f4_enable_clock,
.configure_prescalers = stm32f4_configure_prescalers,
.set_time = stm32f4_set_time,
.get_time = stm32f4_get_time,
.set_date = stm32f4_set_date,
.get_date = stm32f4_get_date,
};

// --- Public functions provided by the port ---
const rtc_port_interface_t* rtc_port_get_api(void) {
    return &stm32f4_port_api;
}

void* rtc_port_get_base_addr(void) {
    return (void*)RTC_BASE;
}
