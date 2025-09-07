################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/Shell/src/FreeRTOS_CLI.c \
../Middleware/Shell/src/FreeRTOS_Shell.c 

OBJS += \
./Middleware/Shell/src/FreeRTOS_CLI.o \
./Middleware/Shell/src/FreeRTOS_Shell.o 

C_DEPS += \
./Middleware/Shell/src/FreeRTOS_CLI.d \
./Middleware/Shell/src/FreeRTOS_Shell.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/Shell/src/%.o Middleware/Shell/src/%.su Middleware/Shell/src/%.cyclo: ../Middleware/Shell/src/%.c Middleware/Shell/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"D:/Users/Documents/Stm/Logic Analyzer/Middleware/Shell/inc" -I"D:/Users/Documents/Stm/Logic Analyzer/Middleware/FreeRTOS/include" -I"D:/Users/Documents/Stm/Logic Analyzer/Middleware/FreeRTOS/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middleware-2f-Shell-2f-src

clean-Middleware-2f-Shell-2f-src:
	-$(RM) ./Middleware/Shell/src/FreeRTOS_CLI.cyclo ./Middleware/Shell/src/FreeRTOS_CLI.d ./Middleware/Shell/src/FreeRTOS_CLI.o ./Middleware/Shell/src/FreeRTOS_CLI.su ./Middleware/Shell/src/FreeRTOS_Shell.cyclo ./Middleware/Shell/src/FreeRTOS_Shell.d ./Middleware/Shell/src/FreeRTOS_Shell.o ./Middleware/Shell/src/FreeRTOS_Shell.su

.PHONY: clean-Middleware-2f-Shell-2f-src

