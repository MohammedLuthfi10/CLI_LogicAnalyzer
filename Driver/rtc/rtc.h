/**
 * @file      rtc.h
 * @brief     Public API for the portable Real-Time Clock (RTC) driver.
 * @author    Your Name
 * @date      2023-10-27
 *
 * @details   This file defines the user-facing functions for setting and
 *            getting the calendar time and date from the RTC peripheral.
 */

#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Opaque Handle Definition --- */

/**
 * @brief Opaque handle representing the RTC instance.
 */
typedef struct rtc_handle_t* rtc_handle_t;

/* --- Public Configuration Types --- */

/** @brief Structure to hold time information. */
typedef struct {
    uint8_t hours;   // 0-23
    uint8_t minutes; // 0-59
    uint8_t seconds; // 0-59
} rtc_time_t;

/** @brief Structure to hold date information. */
typedef struct {
    uint8_t year;    // 0-99 (e.g., 23 for 2023)
    uint8_t month;   // 1-12
    uint8_t day;     // 1-31
    uint8_t weekday; // 1-7 (Monday=1, Sunday=7)
} rtc_date_t;

/**
 * @brief Configuration structure for RTC initialization.
 * @details These values depend on the RTC clock source (e.g., LSE at 32.768 kHz).
 *          For LSE, typical values are async=127, sync=255.
 */
typedef struct {
    uint16_t sync_prescaler;
    uint8_t async_prescaler;
} rtc_config_t;

/* --- Public API Functions --- */

/**
 * @brief Initializes the RTC peripheral.
 * @warning This function requires the backup domain to be write-enabled.
 *
 * @param[in] config Pointer to the configuration structure for the RTC.
 *
 * @return A handle to the RTC instance, or NULL on failure.
 */
rtc_handle_t rtc_init(const rtc_config_t* config);

/**
 * @brief De-initializes the RTC handle.
 *
 * @param[in,out] p_handle Pointer to the handle to de-initialize.
 */
void rtc_deinit(rtc_handle_t* p_handle);

/**
 * @brief Sets the current time in the RTC.
 *
 * @param[in] handle The handle to the RTC instance.
 * @param[in] time Pointer to the time structure to set.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int rtc_set_time(rtc_handle_t handle, const rtc_time_t* time);

/**
 * @brief Gets the current time from the RTC.
 *
 * @param[in] handle The handle to the RTC instance.
 * @param[out] time Pointer to a structure to store the retrieved time.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int rtc_get_time(rtc_handle_t handle, rtc_time_t* time);

/**
 * @brief Sets the current date in the RTC.
 *
 * @param[in] handle The handle to the RTC instance.
 * @param[in] date Pointer to the date structure to set.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int rtc_set_date(rtc_handle_t handle, const rtc_date_t* date);

/**
 * @brief Gets the current date from the RTC.
 *
 * @param[in] handle The handle to the RTC instance.
 * @param[out] date Pointer to a structure to store the retrieved date.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int rtc_get_date(rtc_handle_t handle, rtc_date_t* date);

#endif // RTC_H
