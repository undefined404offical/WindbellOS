# WindbellOS - 64位UEFI启动的命令行操作系统

## 项目概述
WindbellOS是一个正在开发中的64位UEFI启动的命令行操作系统，使用C++编写。

## 当前功能
- UEFI引导程序
- 显示"WindbellOS Started Successfully"消息
- 支持高清显示
- 基本死循环保持系统运行

## 构建要求
- x86_64-w64-mingw32-gcc (交叉编译器)
- make

## 构建步骤
1. 确保已安装x86_64-w64-mingw32-gcc交叉编译器
2. 在项目根目录运行: `make`
3. 构建成功后，UEFI可执行文件将位于 `build/bootx64.efi`
4. 创建UEFI可启动映像: `make mkimg`
5. 映像文件将生成为 `WindbellOS.img`

## 如何运行
### 方法1: 使用QEMU模拟器
1. 安装QEMU和OVMF (UEFI固件):
   ```
   sudo apt-get install qemu-system-x86 ovmf
   ```
2. 运行以下命令启动:
   ```
   make run
   ```

### 方法2: 写入USB设备
1. 将映像写入USB设备 (请将sdX替换为您的USB设备):
   ```
   sudo dd if=WindbellOS.img of=/dev/sdX bs=1M
   ```
2. 从USB设备启动计算机

## 项目结构
```
WindbellOS/
├── src/
│   └── boot/
│       └── main.cpp          # UEFI引导程序主文件
├── include/
│   └── efi.h                 # EFI头文件定义
├── build/                    # 构建输出目录
│   ├── bootx64.efi           # UEFI可执行文件
│   └── main.o                # 编译生成的目标文件
├── mkimg.sh                  # UEFI映像生成脚本
├── Makefile                  # 构建脚本
├── WindbellOS.img            # UEFI可启动映像文件 (运行make mkimg后生成)
└── README.md                 # 本文件
```

## 开发计划
- [x] UEFI引导程序
- [x] 基本显示功能
- [x] 死循环保持系统运行
- [ ] USB键盘支持
- [ ] 基本命令系统
- [ ] 内存管理
- [ ] 文件系统支持

## 许可证
本项目采用MIT许可证。
2025 © WindbellOS 开发团队