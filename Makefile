CC = x86_64-w64-mingw32-gcc
CXX = x86_64-w64-mingw32-g++
LD = x86_64-w64-mingw32-ld
OBJCOPY = x86_64-w64-mingw32-objcopy

# 内核编译器
KERNEL_CC = gcc
KERNEL_CXX = g++
KERNEL_LD = ld

CFLAGS = -ffreestanding -fno-stack-protector -fno-stack-check -fno-pic -fno-pie -mno-red-zone -m64 -c
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -std=c++20
KERNEL_CXXFLAGS = -ffreestanding -fno-stack-protector -fno-stack-check -fno-pic -fno-pie -mno-red-zone -mcmodel=kernel -m64 -c -fno-exceptions -fno-rtti -std=c++20
LDFLAGS = -nostdlib --subsystem 10 -e efi_main
KERNEL_LDFLAGS = -nostdlib -m elf_x86_64 -T kernel.ld

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BOOT_DIR = $(SRC_DIR)/boot
KERNEL_DIR = $(SRC_DIR)/kernel

BOOT_SOURCES = $(wildcard $(BOOT_DIR)/*.cpp)
BOOT_OBJECTS = $(BOOT_SOURCES:$(BOOT_DIR)/%.cpp=$(BUILD_DIR)/%.o)

KERNEL_SOURCES = $(wildcard $(KERNEL_DIR)/*.cpp)
KERNEL_OBJECTS = $(KERNEL_SOURCES:$(KERNEL_DIR)/%.cpp=$(BUILD_DIR)/kernel_%.o)

TARGET = $(BUILD_DIR)/bootx64.efi
KERNEL_TARGET = $(BUILD_DIR)/kernel.bin

.PHONY: all clean mkimg kernel

all: $(TARGET)

kernel: $(KERNEL_TARGET)

mkimg: $(TARGET) $(KERNEL_TARGET)
	@echo "创建WindbellOS UEFI可启动映像..."
	./mkimg.sh

$(TARGET): $(BOOT_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

$(KERNEL_TARGET): $(KERNEL_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(KERNEL_LD) $(KERNEL_LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(BOOT_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $<

$(BUILD_DIR)/kernel_%.o: $(KERNEL_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(KERNEL_CXX) $(KERNEL_CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
	rm -f WindbellOS.img

run: $(TARGET)
	qemu-system-x86_64 -bios OVMF.fd -hda WindbellOS.img