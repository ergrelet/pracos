#include <irq.h>
#include <terminal.h>

uint16_t timer_ticks = 0;

/* Handler for IRQ0 */
void
timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 18 == 0)
    {
        terminal_puts("One second has passed\n");
    }
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void
timer_initialize()
{
    /* Installs 'timer_handler' to IRQ0 */
    irq_add_handler(0, timer_handler);
}
