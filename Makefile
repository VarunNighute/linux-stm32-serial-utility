TARGET = stm32_uart

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb \
         -mfloat-abi=soft \
         -Wall -O2 \
         -ffreestanding \
         -nostdlib

LDFLAGS = -T linker.ld \
          -nostdlib

SRCS = startup.c main.c

all: $(TARGET).bin

$(TARGET).elf: $(SRCS) linker.ld
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(TARGET).elf $(TARGET).bin

