/**
 * @file      exti_config.h
 * @brief     Compile-time configuration for the EXTI driver.
 */

#ifndef EXTI_CONFIG_H
#define EXTI_CONFIG_H

/**
 * @brief Defines the maximum number of EXTI lines (handlers) the driver can manage.
 *        There are 16 EXTI lines for GPIOs (0-15).
 */
#define EXTI_MAX_HANDLES 16

#endif // EXTI_CONFIG_H
