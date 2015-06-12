#include <gdt.h>
#include <idt.h>
#include <isrs.h>
#include <irq.h>
#include <io.h>
#include <terminal.h>
#include <timer.h>
#include <keyboard.h>

/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif
 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();
	terminal_puts("Terminal has been initialized\n");
	/* Initialize GDT & IDT */
	gdt_initialize();
	idt_initialize();
	terminal_puts("GDT & IDT have been initialized\n");
	irq_initialize();
	isrs_initialize();
	enable_interrupts();
	
	timer_initialize();
	kbd_initialize();
	terminal_puts("Keyboard has been initialized\n");
	
	terminal_puts("The End\n");
}
