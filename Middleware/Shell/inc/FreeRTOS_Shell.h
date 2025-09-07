


#ifndef THIRD_PARTY_SHELL_INC_FREERTOS_SHELL_H_
#define THIRD_PARTY_SHELL_INC_FREERTOS_SHELL_H_

#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "task.h"
#include "queue.h"
//#include "stm32f407xx.h"
//#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//#include "stm32f4xx_hal_gpio.h"

static BaseType_t prvCommandTaskStats( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

static BaseType_t prvQueueStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
static BaseType_t prvRunTimeStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
static BaseType_t prvHeapStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);


#define CONSOLE_VERSION_MAJOR                   1
#define CONSOLE_VERSION_MINOR                   0

#define MAX_IN_STR_LEN                          300
#define MAX_OUT_STR_LEN                         600
#define MAX_RX_QUEUE_LEN                        300

                                                      /* ASCII code definition */
#define ASCII_TAB                               '\t'  /* Tabulate              */
#define ASCII_CR                                '\r'  /* Carriage return       */
#define ASCII_LF                                '\n'  /* Line feed             */
#define ASCII_BACKSPACE                         '\b'  /* Back space            */
#define ASCII_FORM_FEED                         '\f'  /* Form feed             */
#define ASCII_DEL                               127   /* Delete                */
#define ASCII_CTRL_PLUS_C                         3   /* CTRL + C              */
#define ASCII_NACK                               21   /* Negative acknowledge  */


#endif /* THIRD_PARTY_SHELL_INC_FREERTOS_SHELL_H_ */
