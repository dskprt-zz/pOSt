#include <stddef.h>
#include "../boot/kernel/kernel.h"
#include "keyboard.h"

unsigned char keymap[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=',
    '\b', '\t', '\n',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    'z', 'x', 'c', 'v', 'b', 'n', 'm',
    '[', ']',
    0,
    ';',
    '\'', '`',
    0,
    '\\',
    ',', '.', '/',
    0,
    '*',
    0,	/* Alt */
    ' ',/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0, 0, 0, 0, 0, 0, 0, 0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

int kb_enable() {
	int data = inportb(0x61);
	outportb(0x61, data & 0x7F);

	return 0;
}

int kb_disable() {
	int data = inportb(0x61);
	outportb(0x61, data | 0x80);

	return 0;
}

void kb_handler() {
	unsigned char scancode;

	scancode = inportb(0x60);

	if(scancode & 0x80) {
		/* You can use this one to see if the user released the
		        *  shift, alt, or control keys... */
	} else {
		write(keymap[scancode], strlen(keymap[scancode]));
	}
}
