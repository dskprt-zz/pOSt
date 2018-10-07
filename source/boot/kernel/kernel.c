#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../../input/keyboard.h"
#include "kernel.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define ENTER_KEY_CODE 0x1C

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const size_t SCREEN_SIZE = 80 * 25;

static uint16_t* VIDEO_MEM = (uint16_t*) 0xB8000;

static inline uint16_t vga_entry(unsigned char uc, uint8_t color);

unsigned int cursor_location = 0;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

inline unsigned char inportb(unsigned int port) {
	unsigned char ret;

	asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));

	return ret;
}

inline void outportb(unsigned int port, unsigned char value) {
	asm volatile ("outb %%al,%%dx" : :"d" (port), "a" (value));
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) {
	size_t len = 0;
    
	while (str[len]) {
        len++;
    }

	return len;
}

void terminal_init(void) {    
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
	clear_screen();
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	VIDEO_MEM[index] = vga_entry(c, color);
}
 
void putchar(char c) {
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    
	if(++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
        
		if(++terminal_row == VGA_HEIGHT) {
			terminal_row = 0;
        }
	}
}
 
void write(const char* data, size_t size) {
	for(size_t i = 0; i < size; i++) {
		putchar(data[i]);
    }
}
 
void writestr(const char* data) {
	write(data, strlen(data));
}

void clear_screen(void) {
	unsigned int i = 0;
    
	while(i < SCREEN_SIZE) {
		writestr(" ");
        i++;
	}
}
 
void kernel_main(void) {
	terminal_init();
	kb_enable(); //this causes cpu error probably too

	//writestr("owo\n");

	for(;;) {
		kb_handler(); //this causes cpu error
	}
}
