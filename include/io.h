#ifndef IO_H
#define IO_H

#define PIC1			0x20		/* IO base address for master PIC */
#define PIC2			0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA		(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA		(PIC2+1)
#define PIC_EOI			0x20		/* End-of-interrupt command code */

unsigned char inb(unsigned int port);
void outb(unsigned int port,unsigned char value);
void enable_interrupts();
void disable_interrupts();

#endif
