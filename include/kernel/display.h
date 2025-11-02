#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// 显示相关常量
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// 颜色常量
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_WHITE 15

// 显示函数声明
extern "C" {
    void display_init();
    void display_clear();
    void display_print(const char* str);
    void display_putchar(char c);
    void display_set_color(uint8_t foreground, uint8_t background);
}

#endif // DISPLAY_H