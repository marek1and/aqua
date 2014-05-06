################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utils/subdir.mk
-include settings/subdir.mk
-include net/subdir.mk
-include dev/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
aqua-device.lss \

FLASH_IMAGE += \
aqua-device.hex \

EEPROM_IMAGE += \
aqua-device.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: aqua-device.elf secondary-outputs

# Tool invocations
aqua-device.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,aqua-device.map -mmcu=atmega32 -o "aqua-device.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

aqua-device.lss: aqua-device.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S aqua-device.elf  >"aqua-device.lss"
	@echo 'Finished building: $@'
	@echo ' '

aqua-device.hex: aqua-device.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex aqua-device.elf  "aqua-device.hex"
	@echo 'Finished building: $@'
	@echo ' '

aqua-device.eep: aqua-device.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex aqua-device.elf  "aqua-device.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: aqua-device.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t aqua-device.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) aqua-device.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
