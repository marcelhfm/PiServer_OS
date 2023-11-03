SRC_DIR = src
BUILD_DIR = build
CFILES = $(wildcard $(SRC_DIR)/*.c)
OFILES = $(CFILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
LLVMPATH = /opt/homebrew/opt/llvm/bin
CLANGFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a72+nosimd

all: clean $(BUILD_DIR)/kernel8.img

$(BUILD_DIR)/boot.o: $(SRC_DIR)/boot.s
	$(LLVMPATH)/clang --target=aarch64-elf $(CLANGFLAGS) -c $(SRC_DIR)/boot.s -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(LLVMPATH)/clang --target=aarch64-elf $(CLANGFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel8.img: $(BUILD_DIR)/boot.o $(OFILES)
	$(LLVMPATH)/ld.lld -m aarch64elf -nostdlib $(BUILD_DIR)/boot.o $(OFILES) -T link.ld -o $(BUILD_DIR)/kernel8.elf
	$(LLVMPATH)/llvm-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

clean:
	/bin/rm $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.img > /dev/null 2> /dev/null || true
