#include "../include/kernel/display.h"

// 内核主函数
extern "C" void kernel_main() {
    // 初始化显示
    display_init();
    
    // 显示内核启动消息
    display_print("Windbell kernel starting...");
    
    // 无限循环以保持内核运行
    while (1) {
        // 可以在这里添加内核空闲任务
    }
}