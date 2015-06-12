/* Input a byte from a port */
unsigned char inb(unsigned int port)
{
   unsigned char ret;
   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}

/* Output a byte to a port */
void outb(unsigned int port,unsigned char value)
{
   asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

void
enable_interrupts() {
	__asm__ __volatile__ ("sti"); 
}

void
disable_interrupts() {
	__asm__ __volatile__ ("cli"); 
}
