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
	gdt_initialize();
	idt_initialize();
	isrs_initialize();
	irq_initialize();
	
	terminal_initialize();
	timer_initialize();
	kbd_initialize();
	enable_interrupts();
	
	terminal_puts("pracos kernel\n");

	for(;;);
}
