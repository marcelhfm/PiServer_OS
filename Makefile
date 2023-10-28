CFILES = $(wildcard *.c)
OFILES = $(CFILES:%.c=build/%.o)
LLVMPATH = /opt/homebrew/opt/llvm/bin
CLANGFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a72+nosimd

all: clean build/kernel8.img

build/boot.o: boot.S
	$(LLVMPATH)/clang --target=aarch64-elf $(CLANGFLAGS) -c boot.S -o build/boot.o

build/%.o: %.c
	$(LLVMPATH)/clang --target=aarch64-elf $(CLANGFLAGS) -c $< -o $@

build/kernel8.img: build/boot.o $(OFILES)
	$(LLVMPATH)/ld.lld -m aarch64elf -nostdlib build/boot.o $(OFILES) -T link.ld -o build/kernel8.elf
	$(LLVMPATH)/llvm-objcopy -O binary build/kernel8.elf build/kernel8.img

clean:
	/bin/rm build/kernel8.elf build/*.o build/*.img > /dev/null 2> /dev/null || true
