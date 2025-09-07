################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dma.c \
../Src/flash.c \
../Src/gpio.c \
../Src/hash.c \
../Src/main.c \
../Src/nvic.c \
../Src/pwr.c \
../Src/rcc.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timer.c \
../Src/usart.c 

OBJS += \
./Src/dma.o \
./Src/flash.o \
./Src/gpio.o \
./Src/hash.o \
./Src/main.o \
./Src/nvic.o \
./Src/pwr.o \
./Src/rcc.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timer.o \
./Src/usart.o 

C_DEPS += \
./Src/dma.d \
./Src/flash.d \
./Src/gpio.d \
./Src/hash.d \
./Src/main.d \
./Src/nvic.d \
./Src/pwr.d \
./Src/rcc.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timer.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"D:/Users/Documents/Stm/Logic Analyzer/Middleware/FreeRTOS/include" -I"D:/Users/Documents/Stm/Logic Analyzer/Middleware/Shell/inc" -I"D:/Users/Documents/Stm/Logic Analyzer/Middleware/FreeRTOS/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/dma.cyclo ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/flash.cyclo ./Src/flash.d ./Src/flash.o ./Src/flash.su ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/hash.cyclo ./Src/hash.d ./Src/hash.o ./Src/hash.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/nvic.cyclo ./Src/nvic.d ./Src/nvic.o ./Src/nvic.su ./Src/pwr.cyclo ./Src/pwr.d ./Src/pwr.o ./Src/pwr.su ./Src/rcc.cyclo ./Src/rcc.d ./Src/rcc.o ./Src/rcc.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timer.cyclo ./Src/timer.d ./Src/timer.o ./Src/timer.su ./Src/usart.cyclo ./Src/usart.d ./Src/usart.o ./Src/usart.su

.PHONY: clean-Src

