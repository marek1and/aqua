################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../dev/adc.c \
../dev/buttons.c \
../dev/ds18b20.c \
../dev/eeprom.c \
../dev/hd44780.c \
../dev/relay.c \
../dev/sound.c \
../dev/time.c \
../dev/twi.c \
../dev/usart.c 

OBJS += \
./dev/adc.o \
./dev/buttons.o \
./dev/ds18b20.o \
./dev/eeprom.o \
./dev/hd44780.o \
./dev/relay.o \
./dev/sound.o \
./dev/time.o \
./dev/twi.o \
./dev/usart.o 

C_DEPS += \
./dev/adc.d \
./dev/buttons.d \
./dev/ds18b20.d \
./dev/eeprom.d \
./dev/hd44780.d \
./dev/relay.d \
./dev/sound.d \
./dev/time.d \
./dev/twi.d \
./dev/usart.d 


# Each subdirectory must supply rules for building sources it contributes
dev/%.o: ../dev/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


