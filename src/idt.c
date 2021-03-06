#include <idt.h>
#include <misc.h>

/* Defines an IDT entry */
struct idt_entry
{
    uint16_t base_low;
    uint16_t selector;		/* Our kernel segment goes here! */
    uint8_t always0;		/* This will ALWAYS be set to 0! */
    uint8_t flags;			/* Set using the above table! */
    uint16_t base_high;
} __attribute__((packed));

/* This goes into the CPU's IDTR */
struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries at the moment, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
struct idt_entry idt[256];
struct idt_ptr idtr;

/* Loads our IDT, defined in boot.s */
extern void _idt_load();

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void
idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = ((base >> 16) & 0xFFFF);
    idt[num].selector = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Installs the IDT */
void
idt_initialize() {
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtr.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtr.base = (uint32_t)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points the processor's internal register to the new IDT */
    _idt_load();
}
