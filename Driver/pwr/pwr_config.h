/**
 * @file      uart_config.h
 * @brief     Compile-time configuration for the UART driver.
 */

#ifndef UART_CONFIG_H
#define UART_CONFIG_H

/**
 * @brief Defines the maximum number of UART instances the driver can manage.
 * @details This sets the size of the static handle pool. Adjust based on
 *          the number of UART peripherals you intend to use simultaneously.
 */
#define UART_MAX_INSTANCES 3

#endif // UART_CONFIG_H
