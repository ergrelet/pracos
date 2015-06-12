#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include <interrupts.h>

void irq_initialize();
void irq_add_handler(int32_t num, void (*handler)(struct regs *r));
void irq_remove_handler(int32_t num);

#endif
