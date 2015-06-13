#include <terminal.h>
#include <misc.h>
#include <io.h>
#include <stdbool.h>

/* Hardware text mode color constants. */
enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
 
uint8_t
make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
uint16_t
make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void
terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_WHITE, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void
terminal_movecursor(size_t row, size_t column) {
    uint16_t position=(row*80) + column;
 
    // cursor LOW port to vga INDEX register
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position>>8)&0xFF));
}
 
void
terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

//TODO
void
terminal_cls() {

}

void
terminal_scroll() {
	/* Scroll line(i) = line(i+1)*/
	for(size_t y=0; y < VGA_HEIGHT-1; ++y) {
		const size_t curr_row = y * VGA_WIDTH;
		const size_t next_row = (y+1) * VGA_WIDTH;
		for(size_t x=0; x < VGA_WIDTH; ++x) {
			const size_t index = curr_row+x;
			terminal_buffer[index] = terminal_buffer[next_row+x];
		}
	}
	const size_t last_row = (VGA_HEIGHT-1) * VGA_WIDTH;
	/* Clear last line */
	for(size_t x=0; x < VGA_WIDTH; ++x) {
		const size_t index = last_row+x;
		terminal_buffer[index] = '\0';
	}
}
 
void
terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void
terminal_putc(char c) {
	/* New line */
	switch(c) {
		case '\n':
			terminal_column = 0;
			if (terminal_row == VGA_HEIGHT-1) {
				terminal_scroll();
			}
			else {
				terminal_row++;
			}
			break;
		case '\t':
			terminal_column += 4-(terminal_column%4);
			break;
		case '\b':
			if(terminal_column > 0) {
				terminal_putentryat(0, terminal_color, --terminal_column, terminal_row);
			}
			break;
		default: /* Other chars */
			terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
			if (++terminal_column == VGA_WIDTH) {
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT) {
					terminal_row = 0;
				}
			}
	}
	terminal_movecursor(terminal_row, terminal_column);
}
 
void
terminal_puts(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putc(data[i]);
}

void
terminal_putuint32(uint32_t n) {
	uint32_t size = 1;
	while(n/size > 9) {
		size *= 10;
	}
	while(size > 0) {
		uint8_t digit = n/size;
		terminal_putc('0'+digit);
		n -= digit*size;
		size /= 10;
	}
}
