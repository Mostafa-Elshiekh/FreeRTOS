################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include STM32f103_Drivers/subdir.mk
-include FreeRTOS/portable/MemMang/subdir.mk
-include FreeRTOS/portable/GCC/ARM_CM3/subdir.mk
-include FreeRTOS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
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
EXECUTABLES += \
Session2_Led_Button.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Session2_Led_Button.list \

OBJCOPY_BIN += \
Session2_Led_Button.bin \


# All Target
all: Session2_Led_Button.elf secondary-outputs

# Tool invocations
Session2_Led_Button.elf: $(OBJS) $(USER_OBJS) C:\Users\musta\Session2_Led_Button\STM32F103C6TX_FLASH.ld
	arm-none-eabi-gcc -o "Session2_Led_Button.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\musta\Session2_Led_Button\STM32F103C6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Session2_Led_Button.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Session2_Led_Button.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Session2_Led_Button.list"
	@echo 'Finished building: $@'
	@echo ' '

Session2_Led_Button.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Session2_Led_Button.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
