#include <irq.h>
#include <io.h>
#include <idt.h>

/* These are own ISRs that point to our special IRQ handler
*  instead of the regular 'fault_handler' function */
extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
void* irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void
irq_add_handler(int32_t num, void (*handler)(struct regs *r))
{
    irq_routines[num] = handler;
}

/* This clears the handler for a given IRQ */
void
irq_remove_handler(int32_t num)
{
    irq_routines[num] = 0;
}

/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 */
void
irq_remap(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void
irq_initialize()
{
    irq_remap();

    idt_set_gate(32, (uint32_t)_irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)_irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)_irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)_irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)_irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)_irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)_irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)_irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)_irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)_irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)_irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)_irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)_irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)_irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)_irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)_irq15, 0x08, 0x8E);
}

void
_irq_handler(struct regs *r)
{
    /* This is a blank function pointer */
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outb(PIC2, PIC_EOI);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outb(PIC1, PIC_EOI);
}
