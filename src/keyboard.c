#include <io.h>
#include <irq.h>
#include <terminal.h>
#include <misc.h>
#include <stdbool.h>

#define KBD_DATA	0x60
#define KBD_STATUS	0x64

bool alt_down = 0;
bool ctrl_down = 0;
bool shift_down = 0;

unsigned char kbdfr[128] =
{
    0,  27, '&', ' ', '\"', '\'', '(', '-', ' ', '_',	/* 9 */
  ' ', ' ', ')', '=', '\b',							/* Backspace */
  '\t',												/* Tab */
  'a', 'z', 'e', 'r',								/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',		/* Enter key */
    0,												/* 29   - Control */
  'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',	/* 39 */
 ' ', '*',   0,									/* Left shift */
 '<', 'w', 'x', 'c', 'v', 'b', 'n',				/* 49 */
  ',', ';', ':', '!',   0,							/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
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

unsigned char kbdfr_shift[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', ' ', '+', '\b',							/* Backspace */
  '\t',												/* Tab */
  'A', 'Z', 'E', 'R',								/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', ' ', ' ', '\n',		/* Enter key */
    0,												/* 29   - Control */
  'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M',	/* 39 */
 ' ', '*',   0,									/* Left shift */
 '>', 'W', 'X', 'C', 'V', 'B', 'N',				/* 49 */
  '?', '.', '/', ' ',   0,							/* Right shift */
  ' ',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
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

// TODO: Don't use terminal_putc and put chars in a stream instead ?
void
kbd_main(unsigned char scancode) {
	if(scancode & 0x80) { /* Key released */
		scancode = scancode ^ 0x80;
		switch(scancode) {
			case 42: /* LSHIFT */
			case 54: /* RSHIFT */
				shift_down = false;
				break;
			case 29: /* CTRL */
				ctrl_down = false;
				break;
			case 56: /* ALT */
				alt_down = false;
				break;
		}
	}
	else { /* Key pressed */
		switch(scancode) {
			case 42: /* LSHIFT */
			case 54: /* RSHIFT */
				shift_down = true;
				break;
			case 29: /* CTRL */
				ctrl_down = true;
				break;
			case 56: /* ALT */
				alt_down = true;
				break;
			default:
				if(!shift_down) {
					terminal_putc(kbdfr[scancode]);
				}
				else {
					terminal_putc(kbdfr_shift[scancode]);
				}
		}
	}
}

/* Handler for IRQ1 */
void
kbd_handler(struct regs *r) {	
	while((inb(0x64)&1) == 0); // Waits for the keyboard to be available
	unsigned char scancode = inb(KBD_DATA);
	kbd_main(scancode); 
}

void
kbd_initialize() {
	irq_add_handler(1, kbd_handler);
}

