#include "../include/kernel/display.h"
#include <cstddef>

// VGA显示缓冲区地址
static volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
static uint8_t terminal_color = 0;
static size_t terminal_row = 0;
static size_t terminal_column = 0;

// 创建VGA颜色条目
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

// 初始化显示
extern "C" void display_init() {
    terminal_color = VGA_COLOR_WHITE | VGA_COLOR_BLACK << 4;
    terminal_row = 0;
    terminal_column = 0;
    display_clear();
}

// 清空屏幕
extern "C" void display_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

// 设置光标位置
static void terminal_set_cursor(size_t row, size_t column) {
    terminal_row = row;
    terminal_column = column;
}

// 滚动屏幕
static void terminal_scroll() {
    // 将所有行向上移动一行
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t src_index = (y + 1) * VGA_WIDTH + x;
            const size_t dst_index = y * VGA_WIDTH + x;
            vga_buffer[dst_index] = vga_buffer[src_index];
        }
    }
    
    // 清空最后一行
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        vga_buffer[index] = vga_entry(' ', terminal_color);
    }
    
    // 将光标移到最后一行
    terminal_row = VGA_HEIGHT - 1;
    terminal_column = 0;
}

// 输出一个字符
extern "C" void display_putchar(char c) {
    // 处理换行符
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row == VGA_HEIGHT) {
            terminal_scroll();
        }
        return;
    }
    
    // 输出字符
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    vga_buffer[index] = vga_entry((unsigned char)c, terminal_color);
    
    // 更新光标位置
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
        }
    }
}

// 输出字符串
extern "C" void display_print(const char* str) {
    size_t i = 0;
    while (str[i]) {
        display_putchar(str[i]);
        i++;
    }
}

// 设置颜色
extern "C" void display_set_color(uint8_t foreground, uint8_t background) {
    terminal_color = foreground | background << 4;
}