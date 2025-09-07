//
//
//
//#include "FreeRTOS_Shell.h"
//
//UART_HandleTypeDef *pxUartDevHandle;
//char cRxData;
//QueueHandle_t xQueueRxHandle;
//
//
//static const char *pcWelcomeMsg = "Welcome to the console. Enter 'help' to view a list of available commands.\n";
//
//static const char *prvpcTaskListHeader = "Task states: Bl = Blocked, Re = Ready, Ru = Running, De = Deleted,  Su = Suspended\n\n"\
//                                         "Task name         State  Priority  Stack remaining  CPU usage  Runtime(us)\n"\
//                                         "================= =====  ========  ===============  =========  ===========\n";
//static const char *prvpcPrompt = "#cmd: ";
//
//static const CLI_Command_Definition_t xCommands[] =
//{
//    {
//        "stats",
//        "\r\nstats:\r\n Displays a table with the state of each FreeRTOS task.\r\n",
//        prvCommandTaskStats,
//        0
//    },
//
//	{
//			"heapstats",
//			    "heapstats: Displays free heap size\r\n",
//			    prvHeapStatsCommand,
//			    0
//	},
//	{
//			"runstats",
//			    "runstats: Displays run-time statistics\r\n",
//			    prvRunTimeStatsCommand,
//			    0
//	},
//	{
//			"queuestats",
//			    "queuestats: Displays number of messages in test queue\r\n",
//			    prvQueueStatsCommand,
//			    0
//	},
//    { NULL, NULL, NULL, 0 }
//};
//
//
///**
//*   @brief  This function is executed in case of error occurrence.
//*   @retval None
//*/
//static const char *prvpcMapTaskState(eTaskState eState)
//{
//    switch (eState)
//    {
//        case     eReady: return "Re";
//        case   eRunning: return "Ru";
//        case   eDeleted: return "De";
//        case   eBlocked: return "Bl";
//        case eSuspended: return "S";
//        default: return "??";
//    }
//}
//
///* CLI command to display heap usage */
//static BaseType_t prvHeapStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {
//    snprintf(pcWriteBuffer, xWriteBufferLen, "Free Heap: %u bytes\r\n", xPortGetFreeHeapSize());
//    return pdFALSE;
//}
//
/////* CLI command to display run-time stats */
////static BaseType_t prvRunTimeStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {
////    	vTaskGetRunTimeStats(pcWriteBuffer);
////    //uxTaskGetSystemState()
////    return pdFALSE;
////}
//
//static BaseType_t prvRunTimeStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
//{
//    const UBaseType_t maxTasks = 10;
//    TaskStatus_t taskStatusArray[maxTasks];
//    UBaseType_t taskCount;
//    uint32_t totalRunTime;
//
//    // Get task stats
//    taskCount = uxTaskGetSystemState(taskStatusArray, maxTasks, &totalRunTime);
//
//    // Ensure output fits
//    size_t offset = 0;
//    offset += snprintf(pcWriteBuffer + offset, xWriteBufferLen - offset,
//                       "Task         State Prio Stack Time%%\r\n");
//
//    for (UBaseType_t i = 0; i < taskCount && offset < xWriteBufferLen; i++)
//    {
//        const char *state;
//
//        switch (taskStatusArray[i].eCurrentState) {
//            case eRunning:   state = "RUN"; break;
//            case eReady:     state = "RDY"; break;
//            case eBlocked:   state = "BLK"; break;
//            case eSuspended: state = "SUS"; break;
//            case eDeleted:   state = "DEL"; break;
//            default:         state = "UNK"; break;
//        }
//        float cpuPercent = (totalRunTime > 0) ?
//            (100.0f * taskStatusArray[i].ulRunTimeCounter / totalRunTime) : 0.0f;
//
//        offset += snprintf(&pcWriteBuffer[offset],
//                           xWriteBufferLen - offset,
//                           "%-12s %-4s %-4u %-5lu %6.2f%%\r\n",
//                           taskStatusArray[i].pcTaskName,
//                           state,
//                           taskStatusArray[i].uxCurrentPriority,
//                           (uint32_t)taskStatusArray[i].usStackHighWaterMark,
//                           cpuPercent);
//
//    }
//
//    return pdFALSE; // output fits in one call
//}
//
//static BaseType_t prvQueueStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) {
//    if (xQueueRxHandle == NULL) {
//        snprintf(pcWriteBuffer, xWriteBufferLen, "Queue not initialized\r\n");
//    } else {
//        snprintf(pcWriteBuffer, xWriteBufferLen, "Messages waiting: %u\r\n", uxQueueMessagesWaiting(xQueueRxHandle));
//    }
//    return pdFALSE;
//}
//
//
//void GPIO_SetBits(GPIO_TypeDef *port, uint16_t pin) {
//    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
//}
//
//void GPIO_ResetBits(GPIO_TypeDef *port, uint16_t pin) {
//    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
//}
//
//static BaseType_t gpioControlCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
//{
//    const char *colorArg;
//    const char *stateArg;
//
//    // Parse arguments
//    colorArg = FreeRTOS_CLIGetParameter(pcCommandString, 1, NULL);
//    stateArg = FreeRTOS_CLIGetParameter(pcCommandString, 2, NULL);
//
//    if (colorArg == NULL || stateArg == NULL) {
//        snprintf(pcWriteBuffer, xWriteBufferLen, "Error: Missing parameters.\r\n");
//        return pdFALSE;
//    }
//
//    // Handle color + state
//    GPIO_TypeDef *port = GPIOD;
//    uint16_t pin = 0;
//
//    if (strncmp(colorArg, "green", 5) == 0)       pin = GPIO_PIN_12;
//    else if (strncmp(colorArg, "orange", 6) == 0) pin = GPIO_PIN_13;
//    else if (strncmp(colorArg, "red", 3) == 0)     pin = GPIO_PIN_14;
//    else if (strncmp(colorArg, "blue", 4) == 0)    pin = GPIO_PIN_15;
//    else {
//        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid color.\r\n");
//        return pdFALSE;
//    }
//
//    if (strncmp(stateArg, "on", 2) == 0) {
//        GPIO_SetBits(port, pin);
//        snprintf(pcWriteBuffer, xWriteBufferLen, "%s LED turned ON.\r\n", colorArg);
//    } else if (strncmp(stateArg, "off", 3) == 0) {
//        GPIO_ResetBits(port, pin);
//        snprintf(pcWriteBuffer, xWriteBufferLen, "%s LED turned OFF.\r\n", colorArg);
//    } else {
//        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid state. Use 'on' or 'off'.\r\n");
//        return pdFALSE;
//    }
//
//    return pdFALSE; // pdFALSE = no more output
//}
//
//
///**
//* @brief Command that gets task statistics.
//* @param *pcWriteBuffer FreeRTOS CLI write buffer.
//* @param xWriteBufferLen Length of write buffer.
//* @param *pcCommandString pointer to the command name.
//* @retval FreeRTOS status
//*/
//static BaseType_t prvCommandTaskStats( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
//{
//    static uint32_t uTaskIndex = 0;
//    static uint32_t uTotalOfTasks = 0;
//    static uint32_t uTotalRunTime = 1;
//    TaskStatus_t *pxTmpTaskStatus = NULL;
//    static TaskStatus_t *pxTaskStatus = NULL;
//
//    if (pxTaskStatus == NULL)
//    {
//        uTotalOfTasks = uxTaskGetNumberOfTasks();
//        pxTaskStatus = pvPortMalloc(uTotalOfTasks * sizeof(TaskStatus_t));
//        if (pxTaskStatus == NULL)
//        {
//           snprintf(pcWriteBuffer, xWriteBufferLen, "Error: Not enough memory for task allocation");
//           goto out_cmd_task_stats;
//        }
//        uTotalOfTasks = uxTaskGetSystemState(pxTaskStatus, uTotalOfTasks, &uTotalRunTime);
//        uTaskIndex = 0;
//        uTotalRunTime /= 100;
//        snprintf(pcWriteBuffer, xWriteBufferLen, prvpcTaskListHeader);
//    }
//    else
//    {
//        memset(pcWriteBuffer, 0x00, MAX_OUT_STR_LEN);
//        /* Prevent from zero division */
//        if (!uTotalRunTime)
//        {
//            uTotalRunTime = 1;
//        }
//
//        pxTmpTaskStatus = &pxTaskStatus[uTaskIndex];
//        if (pxTmpTaskStatus->ulRunTimeCounter / uTotalRunTime < 1)
//        {
//         snprintf(pcWriteBuffer, xWriteBufferLen,
//                 "%-16s  %5s  %8lu  %14dB       < 1%%  %11lu\n",
//                 pxTmpTaskStatus->pcTaskName,
//                 prvpcMapTaskState(pxTmpTaskStatus->eCurrentState),
//                 pxTmpTaskStatus->uxCurrentPriority,
//                 pxTmpTaskStatus->usStackHighWaterMark,
//                 pxTmpTaskStatus->ulRunTimeCounter);
//        }
//        else
//        {
//            snprintf(pcWriteBuffer, xWriteBufferLen,
//                    "%-16s  %5s  %8lu  %14dB  %8lu%%  %11lu\n",
//                    pxTmpTaskStatus->pcTaskName,
//                    prvpcMapTaskState(pxTmpTaskStatus->eCurrentState),
//                    pxTmpTaskStatus->uxCurrentPriority,
//                    pxTmpTaskStatus->usStackHighWaterMark,
//                    pxTmpTaskStatus->ulRunTimeCounter / uTotalRunTime,
//                    pxTmpTaskStatus->ulRunTimeCounter);
//        }
//        uTaskIndex++;
//    }
//
//    /* Check if there is more tasks to be process */
//    if (uTaskIndex < uTotalOfTasks)
//       return pdTRUE;
//    else
//    {
//out_cmd_task_stats :
//        if (pxTaskStatus != NULL)
//        {
//            vPortFree(pxTaskStatus);
//            pxTaskStatus = NULL;
//        }
//        return pdFALSE;
//    }
//}
//
///**
//* @brief Reads from UART RX buffer. Reads one bye at the time.
//* @param *cReadChar pointer to where data will be stored.
//* @retval FreeRTOS status
//*/
//static BaseType_t xConsoleRead(uint8_t *cReadChar, size_t xLen)
//{
//    BaseType_t xRetVal = pdFALSE;
//
//    if (xQueueRxHandle == NULL || cReadChar == NULL)
//    {
//        return xRetVal;
//    }
//
//    /* Block until the there is input from the user */
//    return xQueueReceive(xQueueRxHandle, cReadChar, portMAX_DELAY);
//}
//
///**
//* @brief Write to UART TX
//* @param *buff buffer to be written.
//* @retval HAL status
//*/
//static HAL_StatusTypeDef vConsoleWrite(const char *buff)
//{
//    HAL_StatusTypeDef status;
//    size_t len = strlen(buff);
//
//    if (pxUartDevHandle == NULL || *buff == '\0' || len < 1)
//    {
//        return HAL_ERROR;
//    }
//
//    status = HAL_UART_Transmit(pxUartDevHandle, (uint8_t *)buff, strlen(buff), portMAX_DELAY);
//    if (status != HAL_OK)
//    {
//    	return HAL_ERROR;
//    }
//    return status;
//}
//
///**
//* @brief Enables UART RX reception.
//* @param void
//* @retval void
//*/
//void vConsoleEnableRxInterrupt(void)
//{
//    if (pxUartDevHandle == NULL)
//    {
//        return;
//    }
//    /* UART Rx IT is enabled by reading a character */
//    HAL_UART_Receive_IT(pxUartDevHandle,(uint8_t*)&cRxData, 1);
//}
//
//
///**
//* @brief Task to handle user commands via serial communication.
//* @param *pvParams Data passed at task creation.
//* @retval void
//*/
//void vTaskConsole(void *pvParams)
//{
//    char cReadCh = '\0';
//    uint8_t uInputIndex = 0;
//    BaseType_t xMoreDataToProcess;
//    char pcInputString[MAX_IN_STR_LEN];
//    char pcPrevInputString[MAX_IN_STR_LEN];
//    char pcOutputString[MAX_OUT_STR_LEN];
//
//    memset(pcInputString, 0x00, MAX_IN_STR_LEN);
//    memset(pcPrevInputString, 0x00, MAX_IN_STR_LEN);
//    memset(pcOutputString, 0x00, MAX_OUT_STR_LEN);
//
//    /* Create a queue to store characters from RX ISR */
//    xQueueRxHandle = xQueueCreate(MAX_RX_QUEUE_LEN, sizeof(char));
//    if (xQueueRxHandle == NULL)
//    {
//        goto out_task_console;
//    }
//
//    vConsoleWrite(pcWelcomeMsg);
//    vConsoleEnableRxInterrupt();
//    vConsoleWrite(prvpcPrompt);
//
//    while(1)
//    {
//        /* Block until there is a new character in RX buffer */
//        xConsoleRead((uint8_t*)(&cReadCh), sizeof(cReadCh));
//
//        switch (cReadCh)
//        {
//            case ASCII_CR:
//            case ASCII_LF:
//                if (uInputIndex != 0)
//                {
//                    vConsoleWrite("\n\n");
//                    strncpy(pcPrevInputString, pcInputString, MAX_IN_STR_LEN);
//                    do
//                    {
//                        xMoreDataToProcess = FreeRTOS_CLIProcessCommand
//                                            (
//                                                pcInputString,    /* Command string*/
//                                                pcOutputString,   /* Output buffer */
//                                                MAX_OUT_STR_LEN   /* Output buffer size */
//                                            );
//                        vConsoleWrite(pcOutputString);
//                    } while (xMoreDataToProcess != pdFALSE);
//                }
//                uInputIndex = 0;
//                memset(pcInputString, 0x00, MAX_IN_STR_LEN);
//                memset(pcOutputString, 0x00, MAX_OUT_STR_LEN);
//                vConsoleWrite("\n");
//                vConsoleWrite(prvpcPrompt);
//                break;
//            case ASCII_FORM_FEED:
//                vConsoleWrite("\x1b[2J\x1b[0;0H");
//                vConsoleWrite("\n");
//                vConsoleWrite(prvpcPrompt);
//                break;
//            case ASCII_CTRL_PLUS_C:
//                uInputIndex = 0;
//                memset(pcInputString, 0x00, MAX_IN_STR_LEN);
//                vConsoleWrite("\n");
//                vConsoleWrite(prvpcPrompt);
//                break;
//            case ASCII_DEL:
//            case ASCII_NACK:
//            case ASCII_BACKSPACE:
//                if (uInputIndex > 0)
//                {
//                    uInputIndex--;
//                    pcInputString[uInputIndex] = '\0';
//                    vConsoleWrite("\b \b");
//                }
//                break;
//            case ASCII_TAB:
//                while (uInputIndex)
//                {
//                    uInputIndex--;
//                    vConsoleWrite("\b \b");
//                }
//                strncpy(pcInputString, pcPrevInputString, MAX_IN_STR_LEN);
//                uInputIndex = (unsigned char)strlen(pcInputString);
//                vConsoleWrite(pcInputString);
//                break;
//            default:
//                /* Check if read character is between [Space] and [~] in ASCII table */
//                if (uInputIndex < (MAX_IN_STR_LEN - 1 ) && (cReadCh >= 32 && cReadCh <= 126))
//                {
//                    pcInputString[uInputIndex] = cReadCh;
//                    vConsoleWrite(pcInputString + uInputIndex);
//                    uInputIndex++;
//                }
//                break;
//        }
//    }
//
//out_task_console:
//    if (xQueueRxHandle)
//    {
//        vQueueDelete(xQueueRxHandle);
//    }
//    vTaskDelete(NULL);
//}
//
//
///**
//* @brief Initialize the console by registering all commands and creating a task.
//* @param usStackSize Task console stack size
//* @param uxPriority Task console priority
//* @param *pxUartHandle Pointer for uart handle.
//* @retval FreeRTOS status
//*/
////BaseType_t xbspConsoleInit(uint16_t usStackSize, UBaseType_t uxPriority, UART_HandleTypeDef *pxUartHandle)
////{
////    const CLI_Command_Definition_t *pCommand;
////
////    if (pxUartHandle == NULL)
////    {
////        return pdFALSE;
////    }
////    pxUartDevHandle = pxUartHandle;
////
////    /* Register all commands that can be accessed by the user */
////    for (pCommand = xCommands; pCommand->pcCommand != NULL; pCommand++)
////    {
////        FreeRTOS_CLIRegisterCommand(pCommand);
////    }
////    return xTaskCreate(vTaskConsole,"CLI", usStackSize, NULL, uxPriority, NULL);
////}
////
/////**
////* @brief Callback for UART RX, triggered any time there is a new character.
////* @param *huart Pointer to the uart handle.
////* @retval void
////*/
////void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
////{
////    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
////
////    if (xQueueRxHandle != NULL)
////    {
////        xQueueSendToBackFromISR(xQueueRxHandle, &cRxData, &pxHigherPriorityTaskWoken);
////    }
////    vConsoleEnableRxInterrupt();
////}
//
