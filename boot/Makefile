###################################
#           PROJECT NAME          
###################################
PROJECT     := boot
MAIN_SRC    := boot.c

###################################
#          TOOLCHAINS PATH        
###################################
TOOLCHAINS_PATH := /home/bdang/toolchains/gcc-arm-none-eabi-14.3
PREFIX          := arm-none-eabi

CC := $(TOOLCHAINS_PATH)/bin/$(PREFIX)-gcc
LD := $(CC)
AS := $(TOOLCHAINS_PATH)/bin/$(PREFIX)-as
CP := $(TOOLCHAINS_PATH)/bin/$(PREFIX)-objcopy
SZ := $(TOOLCHAINS_PATH)/bin/$(PREFIX)-size

###################################
#         STARTUP & LINKER        
###################################
STARTUP_FILE := startup_stm32f10x_md.s
LD_SCRIPT    := stm32_flash_boot.ld

###################################
#           DIRECTORIES           
###################################
BUILD_DIR      := z_build
BOOT_BUILD_DIR := $(BUILD_DIR)/boot

CMSIS_DIR      := CMSIS/inc
DEVICE_INC     := Device/inc
DEVICE_SRC     := Device/src
STARTUP_DIR    := Device/startup
LINKER_DIR     := Device/linker

SPL_INC        := SPL/inc
SPL_SRC        := SPL/src

SPI_DIR        := spi_engine
W25_DIR        := w25q32_engine
DBG_DIR        := debugger
USART_DIR      := usart_engine

###################################
#          INCLUDE PATHS          
###################################
INCLUDES = \
    -I$(CMSIS_DIR) \
    -I$(DEVICE_INC) \
    -I$(SPL_INC) \
    -I$(DBG_DIR) \
    -I$(SPI_DIR) \
    -I$(W25_DIR) \
    -I$(USART_DIR)

###################################
#           SOURCE FILES          
###################################
BOOT_C_SOURCES   := $(MAIN_SRC)
DEVICE_C_SOURCES := $(shell find $(DEVICE_SRC) -name "*.c")
SPL_C_SOURCES    := $(shell find $(SPL_SRC) -name "*.c")
SPI_C_SOURCES    := $(shell find $(SPI_DIR) -name "*.c")
W25_C_SOURCES    := $(shell find $(W25_DIR) -name "*.c")
DBG_C_SOURCES    := $(shell find $(DBG_DIR) -name "*.c")
USART_C_SOURCES  := $(shell find $(USART_DIR) -name "*.c")

###################################
#           OBJECT FILES          
###################################
BOOT_OBJECTS   := $(BOOT_C_SOURCES:%.c=$(BOOT_BUILD_DIR)/%.o)
DEVICE_OBJECTS := $(DEVICE_C_SOURCES:$(DEVICE_SRC)/%.c=$(BOOT_BUILD_DIR)/Device/%.o)
SPL_OBJECTS    := $(SPL_C_SOURCES:$(SPL_SRC)/%.c=$(BOOT_BUILD_DIR)/SPL/%.o)
SPI_OBJECTS    := $(SPI_C_SOURCES:$(SPI_DIR)/%.c=$(BOOT_BUILD_DIR)/spi_engine/%.o)
W25_OBJECTS    := $(W25_C_SOURCES:$(W25_DIR)/%.c=$(BOOT_BUILD_DIR)/w25q32_engine/%.o)
DBG_OBJECTS    := $(DBG_C_SOURCES:$(DBG_DIR)/%.c=$(BOOT_BUILD_DIR)/debugger/%.o)
USART_OBJECTS  := $(USART_C_SOURCES:$(USART_DIR)/%.c=$(BOOT_BUILD_DIR)/usart_engine/%.o)

STARTUP_OBJECT := $(BUILD_DIR)/startup/startup.o

ALL_OBJECTS := \
    $(BOOT_OBJECTS) \
    $(DEVICE_OBJECTS) \
    $(SPL_OBJECTS) \
    $(SPI_OBJECTS) \
    $(W25_OBJECTS) \
    $(DBG_OBJECTS) \
    $(USART_OBJECTS) \
    $(STARTUP_OBJECT)

###################################
#              FLAGS              
###################################
CFLAGS = -mcpu=cortex-m3 -mthumb
CFLAGS += -Wall -Wextra -O0 -g3
CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += $(INCLUDES)

ASFLAGS = -mcpu=cortex-m3 -mthumb -g

LDFLAGS = -mcpu=cortex-m3 -mthumb
LDFLAGS += -T$(LINKER_DIR)/$(LD_SCRIPT)
LDFLAGS += --specs=nano.specs --specs=nosys.specs
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref
LDFLAGS += -Wl,--gc-sections

###################################
#            BUILD RULES          
###################################
.PHONY: all clean flash debug bin hex

all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/$(PROJECT).hex
	@echo "Bootloader build complete!"

bin: $(BUILD_DIR)/$(PROJECT).bin
hex: $(BUILD_DIR)/$(PROJECT).hex

###################################
#          COMPILE RULES          
###################################

$(BOOT_BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_BUILD_DIR)/Device/%.o: $(DEVICE_SRC)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_BUILD_DIR)/SPL/%.o: $(SPL_SRC)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_BUILD_DIR)/spi_engine/%.o: $(SPI_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_BUILD_DIR)/w25q32_engine/%.o: $(W25_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_BUILD_DIR)/debugger/%.o: $(DBG_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_BUILD_DIR)/usart_engine/%.o: $(USART_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Startup
$(STARTUP_OBJECT): $(STARTUP_DIR)/$(STARTUP_FILE)
	@mkdir -p $(BUILD_DIR)/startup
	@echo "AS $<"
	$(CC) $(ASFLAGS) -c $< -o $@

###################################
#               LINK              
###################################
$(BUILD_DIR)/$(PROJECT).elf: $(ALL_OBJECTS)
	@echo "LD $@"
	$(LD) $(ALL_OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

###################################
#            BIN & HEX            
###################################
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	@echo "OBJCOPY BIN $@"
	$(CP) -O binary $< $@

$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@echo "OBJCOPY HEX $@"
	$(CP) -O ihex $< $@

###################################
#              CLEAN              
###################################
clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR)

###################################
#             FLASH               
###################################
flash: $(BUILD_DIR)/$(PROJECT).bin
	@echo "Flashing bootloader..."
	st-flash write $< 0x08000000

###################################
#              DEBUG              
###################################
debug: $(BUILD_DIR)/$(PROJECT).elf
	gdb-multiarch $< \
	-ex "target extended-remote localhost:3333" \
	-ex "monitor reset halt" \
	-ex "load" \
	-ex "break main" \
	-ex "continue"
