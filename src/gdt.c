#include <gdt.h>

/* Defines a GDT entry */
struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

/* This goes into the CPU's GDTR */
struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[3];
struct gdt_ptr gdtr;

/* We use this to properly reload the new segment registers */
/* Defined in boot.s */
extern void _gdt_flush();

/* Setup a descriptor in the Global Descriptor Table */
void
gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

/* This will setup the special GDT pointer, set up the first 
*  3 entries in our GDT, and then finally call gdt_flush() */
void
gdt_initialize()
{
    /* Setup the GDT pointer and limit */
    gdtr.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdtr.base = (uint32_t)&gdt;

    /* First entry, NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor. */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Flush out the old GDT and install the new changes! */
    _gdt_flush();
}
	
		
