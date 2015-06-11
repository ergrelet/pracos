#include <terminal.h>

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
	
	for(size_t i=0; i< 25; ++i) {
		terminal_writestring("Pracos kernel spams\n");
	}
	terminal_writestring("OOPS1\n");
	terminal_writestring("OOPS2\n");
}
