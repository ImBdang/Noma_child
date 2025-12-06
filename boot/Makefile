###################################
#           PROJECT NAME          
###################################
PROJECT     := boot
MAIN_SRC    := boot.c

###################################
#           ARCHITECTURE          
###################################
MCU         := cortex-m3
FPU         :=
FLOAT_ABI   :=

###################################
#         STARTUP & LINKER        
###################################
STARTUP_FILE := startup_stm32f10x_md.s
LD_SCRIPT    := stm32_flash_boot.ld

###################################
#          TOOLCHAINS             
###################################
TOOLCHAINS_PATH    := /home/bdang/toolchains/gcc-arm-none-eabi-14.3
PREFIX             := arm-none-eabi

CC := $(TOOLCHAINS_PATH)/bin/$(PREFIX)-gcc
LD := $(CC)
AS := $(TOOLCHAINS_PATH)/bin/$(PREFIX)-as
CP := $(TOOLCHAINS_PATH)/bin/$(PREFIX)-objcopy
SZ := $(TOOLCHAINS_PATH)/bin/$(PREFIX)-size

###################################
#          DIRECTORIES            
###################################

BUILD_DIR      := z_build
BOOT_BUILD_DIR := $(BUILD_DIR)/boot
CMSIS_DIR      := ./CMSIS/inc
DEVICE_INC     := ./Device/inc
DEVICE_SRC     := ./Device/src
SPL_INC        := ./SPL/inc
SPL_SRC        := ./SPL/src
STARTUP_DIR    := ./Device/startup
LINKER_DIR     := ./Device/linker

###################################
#          INCLUDE PATHS          
###################################
INCDIR += $(CMSIS_DIR)
INCDIR += $(DEVICE_INC)
INCDIR += $(SPL_INC)
INCDIR += .

###################################
#          SOURCE FILES           
###################################

BOOT_C_SOURCES   := $(MAIN_SRC)
DEVICE_C_SOURCES := $(shell find $(DEVICE_SRC) -name "*.c")
SPL_C_SOURCES    := $(shell find $(SPL_SRC) -name "*.c")

###################################
#          OBJECTS                
###################################

BOOT_OBJECTS   := $(BOOT_C_SOURCES:%.c=$(BOOT_BUILD_DIR)/%.o)
DEVICE_OBJECTS := $(DEVICE_C_SOURCES:$(DEVICE_SRC)/%.c=$(BOOT_BUILD_DIR)/Device/%.o)
SPL_OBJECTS    := $(SPL_C_SOURCES:$(SPL_SRC)/%.c=$(BOOT_BUILD_DIR)/SPL/%.o)

STARTUP_OBJECT := $(BUILD_DIR)/startup/startup.o

ALL_OBJECTS := $(BOOT_OBJECTS) $(DEVICE_OBJECTS) $(SPL_OBJECTS) $(STARTUP_OBJECT)

###################################
#            FLAGS                
###################################

CFLAGS := -mcpu=$(MCU) -mthumb $(FPU) $(FLOAT_ABI)
CFLAGS += -Wall -Wextra -O0 -g3
CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += $(addprefix -I,$(INCDIR))

ASFLAGS := -mcpu=$(MCU) -mthumb -g

LDFLAGS := -mcpu=$(MCU) -mthumb
LDFLAGS += -T$(LINKER_DIR)/$(LD_SCRIPT)
LDFLAGS += --specs=nano.specs --specs=nosys.specs
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref
LDFLAGS += -Wl,--gc-sections

###################################
#            BUILD RULES          
###################################

.PHONY: all clean flash bin hex debug

all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/$(PROJECT).hex
	@echo "Bootloader build complete!"

bin: $(BUILD_DIR)/$(PROJECT).bin
hex: $(BUILD_DIR)/$(PROJECT).hex

###################################
#          COMPILE RULES          
###################################

$(BOOT_BUILD_DIR)/%.o: %.c
	@mkdir -p $(subst /boot.c,,$(dir $@))
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_BUILD_DIR)/Device/%.o: $(DEVICE_SRC)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_BUILD_DIR)/SPL/%.o: $(SPL_SRC)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Startup assembly
$(STARTUP_OBJECT): $(STARTUP_DIR)/$(STARTUP_FILE)
	@mkdir -p $(BUILD_DIR)/startup
	@echo "AS $<"
	@$(CC) $(ASFLAGS) -c $< -o $@

###################################
#             LINK                
###################################

$(BUILD_DIR)/$(PROJECT).elf: $(ALL_OBJECTS)
	@echo "LD $@"
	@$(LD) $(ALL_OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

###################################
#            BIN & HEX            
###################################

$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	@echo "OBJCOPY $@"
	@$(CP) -O binary $< $@

$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@echo "OBJCOPY $@"
	@$(CP) -O ihex $< $@

###################################
#            CLEAN                
###################################

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

###################################
#            FLASH                
###################################

flash: $(BUILD_DIR)/$(PROJECT).bin
	@echo "Flashing bootloader..."
	st-flash write $< 0x08000000

###################################
#            DEBUG                
###################################

debug: $(BUILD_DIR)/$(PROJECT).elf
	gdb-multiarch $< \
	-ex "target extended-remote localhost:3333" \
	-ex "monitor reset halt" \
	-ex "load" \
	-ex "break main" \
	-ex "continue"
