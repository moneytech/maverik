ASM     := arm-none-eabi-as
CC      := arm-none-eabi-gcc
OBJDUMP := arm-none-eabi-objdump
OBJCPY  := arm-none-eabi-objcopy

SRCDIR  := src/
BLDDIR  := _build/

TARGET  := kernel.img
LINKER  := kernel.ld

OBJ     := $(patsubst $(SRCDIR)%.c,$(BLDDIR)%.o,$(wildcard $(SRCDIR)*.c))
ASMSRC  := boot.o

all: $(BLDDIR) $(TARGET)

$(TARGET) : $(BLDDIR)maverik.elf
	@$(OBJCPY) $(BLDDIR)maverik.elf -O binary $(TARGET)

$(BLDDIR)maverik.elf : $(OBJ) $(LINKER)
	@echo "Linking ..."
	@$(CC) -T $(LINKER) -o $(BLDDIR)maverik.elf -ffreestanding -O2 -nostdlib $(OBJ) $(BLDDIR)$(ASMSRC)

$(BLDDIR)%.o: $(SRCDIR)%.c ASM
	@echo "Compiling" $< "..." 
	@$(CC) -mcpu=arm1176jzf-s -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra -c $< -o $@

ASM: $(BLDDIR)
	@echo "Compiling asm ..."
	@$(ASM) -I $(SRCDIR) $(SRCDIR)*.asm -o $(BLDDIR)$(ASMSRC)

$(BLDDIR):
	@mkdir $@

.PHONY: clean

clean:
	@echo "Cleaning ..."
	@rm -rf $(BLDDIR)
	@rm -f $(TARGET)
