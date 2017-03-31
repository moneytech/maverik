ASM     := armv7a-hardfloat-linux-gnueabi-as
LD      := armv7a-hardfloat-linux-gnueabi-ld
OBJDUMP := armv7a-hardfloat-linux-gnueabi-objdump
OBJCPY  := armv7a-hardfloat-linux-gnueabi-objcopy

SRCDIR  := src/
BLDDIR  := _build/

TARGET  := kernel.img
LIST    := kernel.list
MAP     := kernel.map
LINKER  := kernel.ld

OBJ     := $(patsubst $(SRCDIR)%.asm,$(BLDDIR)%.o,$(wildcard $(SRCDIR)*.asm))

all: $(BLDDIR) $(TARGET) $(LIST)

$(LIST) : $(BLDDIR)output.elf 
	@$(OBJDUMP) -d $(BLDDIR)output.elf > $(LIST)

$(TARGET) : $(BLDDIR)output.elf
	@$(OBJCPY) $(BLDDIR)output.elf -O binary $(TARGET)

$(BLDDIR)output.elf : $(OBJ) $(LINKER)
	@echo "Linking..."
	@$(LD) --no-undefined $(OBJ) -Map $(MAP) -o $(BLDDIR)output.elf -T $(LINKER)

$(BLDDIR)%.o: $(SRCDIR)%.asm $(BUILD)
	@echo "Compiling..."
	@$(ASM) -I $(SRCDIR) $< -o $@

$(BLDDIR):
	@mkdir $@

.PHONY: clean

clean:
	@echo "Cleaning..."
	@rm -rf $(BLDDIR)
	@rm -f $(TARGET)
	@rm -f $(LIST)
	@rm -f $(MAP)
