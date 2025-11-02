#!/bin/bash

# WindbellOS UEFI映像生成脚本
# 此脚本将创建一个FAT32格式的UEFI可启动映像文件

set -e

# 检查必要的工具
command -v dd >/dev/null 2>&1 || { echo "需要安装dd工具"; exit 1; }
command -v mkfs.fat >/dev/null 2>&1 || { echo "需要安装dosfstools (mkfs.fat)"; exit 1; }
command -v mmd >/dev/null 2>&1 || { echo "需要安装mtools (mmd)"; exit 1; }
command -v mcopy >/dev/null 2>&1 || { echo "需要安装mtools (mcopy)"; exit 1; }

# 设置变量
IMAGE_FILE="WindbellOS.img"
BOOT_EFI="build/bootx64.efi"
KERNEL_BIN="build/kernel.bin"
IMAGE_SIZE_MB=64  # 映像文件大小(MB)
SECTOR_SIZE=512   # 扇区大小(字节)

# 计算扇区数量
SECTORS=$((IMAGE_SIZE_MB * 1024 * 1024 / SECTOR_SIZE))

echo "正在创建WindbellOS UEFI可启动映像..."

# 检查bootx64.efi和kernel.bin是否存在
if [ ! -f "$BOOT_EFI" ]; then
    echo "错误: $BOOT_EFI 不存在，请先运行make构建"
    exit 1
fi

if [ ! -f "$KERNEL_BIN" ]; then
    echo "错误: $KERNEL_BIN 不存在，请先运行make kernel构建"
    exit 1
fi

# 创建空映像文件
echo "创建${IMAGE_SIZE_MB}MB的空映像文件..."
dd if=/dev/zero of="$IMAGE_FILE" bs=$SECTOR_SIZE count=$SECTORS

# 格式化为FAT32
echo "格式化为FAT32文件系统..."
mkfs.fat -F 32 "$IMAGE_FILE"

# 创建EFI目录结构
echo "创建EFI目录结构..."
mmd -i "$IMAGE_FILE" ::EFI
mmd -i "$IMAGE_FILE" ::EFI/BOOT

# 复制bootx64.efi到EFI/BOOT/BOOTX64.EFI
echo "复制引导文件..."
mcopy -i "$IMAGE_FILE" "$BOOT_EFI" ::EFI/BOOT/BOOTX64.EFI

# 复制kernel.bin到根目录
echo "复制内核文件..."
mcopy -i "$IMAGE_FILE" "$KERNEL_BIN" ::/KERNEL.BIN

echo "UEFI可启动映像创建完成: $IMAGE_FILE"
