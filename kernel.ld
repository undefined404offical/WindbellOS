ENTRY(kernel_main)

SECTIONS
{
    . = 0x100000; /* 内核加载地址，1MB处 */
    
    .text :
    {
        *(.text)
        *(.rodata*)
    }
    
    .data :
    {
        *(.data)
    }
    
    .bss :
    {
        *(.bss)
        *(COMMON)
    }
    
    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
    }
}