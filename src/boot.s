# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard. We put this into a special
# section so we can force the header to be in the start of the final program.
# You don't need to understand all these details as it is just magic values that
# is documented in the multiboot standard. The bootloader will search for this
# magic sequence and recognize us as a multiboot kernel.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Currently the stack pointer register (esp) points at anything and using it may
# cause massive harm. Instead, we'll provide our own stack. We will allocate
# room for a small temporary stack by creating a symbol at the bottom of it,
# then allocating 16384 bytes for it, and finally creating a symbol at the top.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The linker script specifies _start as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded. It
# doesn't make sense to return from this function as the bootloader is gone.
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp
	call kernel_main
.Lhang:
	jmp .Lhang

# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
.size _start, . - _start

# GTD
.global _gdt_flush
.type _gdt_flush, @function
_gdt_flush:
	lgdt (gdtr)
	ljmp $0x08,$complete_flush
	
complete_flush:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret

# Loads the IDT defined in 'idtr' into the processor.
.global _idt_load
.type _idt_load, @function
_idt_load:
    lidt (idtr)
    ret
    
# ISR
.global _isr0
.global _isr1
.global _isr2
.global _isr3
.global _isr4
.global _isr5
.global _isr6
.global _isr7
.global _isr8
.global _isr9
.global _isr10
.global _isr11
.global _isr12
.global _isr13
.global _isr14
.global _isr15
.global _isr16
.global _isr17
.global _isr18
.global _isr19
.global _isr20
.global _isr21
.global _isr22
.global _isr23
.global _isr24
.global _isr25
.global _isr26
.global _isr27
.global _isr28
.global _isr29
.global _isr30
.global _isr31

#  0: Divide By Zero Exception
_isr0:
    cli
    push 0    # A normal ISR stub that pops a dummy error code to keep a
              # uniform stack frame
    push 0
    jmp isr_common_stub

#  1: Debug Exception
_isr1:
    cli
    push 0
    push 1
    jmp isr_common_stub
    
#  2: Non Maskable Interrupt Exception
_isr2:
    cli
    push 0
    push 2
    jmp isr_common_stub
    
#  3: Breakpoint Exception
_isr3:
    cli
    push 0
    push 3
    jmp isr_common_stub
    
#  4: Into Detected Overflow Exception 
_isr4:
    cli
    push 0
    push 4
    jmp isr_common_stub

#  5: Out of Bounds Exception 
_isr5:
    cli
    push 0
    push 5
    jmp isr_common_stub
    
#  6: Invalid Opcode Exception 
_isr6:
    cli
    push 0
    push 6
    jmp isr_common_stub    
    
#  7: No Coprocessor Exception 
_isr7:
    cli
    push 0
    push 7
    jmp isr_common_stub
    
#  8: Double Fault Exception (With Error Code!)
_isr8:
    cli
    push 8    # Note that we DON'T push a value on the stack in this one!
              # It pushes one already! Use this type of stub for exceptions
              # that pop error 
	jmp isr_common_stub

#  9: Coprocessor Segment Overrun Exception 
_isr9:
    cli
    push 0
    push 9
	jmp isr_common_stub
	
#  10: Bad TSS Exception
_isr10:
    cli
    push 10
	jmp isr_common_stub
	
#  11: Segment Not Present Exception
_isr11:
    cli
    push 11
	jmp isr_common_stub
	
#  12: Stack Fault Exception 
_isr12:
    cli
    push 12
	jmp isr_common_stub
	
#  13: General Protection Fault Exception
_isr13:
    cli
    push 13
	jmp isr_common_stub
	
#  14: Page Fault Exception
_isr14:
    cli
    push 14
	jmp isr_common_stub
	
#  15: Unknown Interrupt Exception
_isr15:
    cli
    push 0
    push 15
	jmp isr_common_stub
	
#  16: Coprocessor Fault Exception
_isr16:
    cli
    push 0
    push 16
	jmp isr_common_stub
	
#  17: Alignment Check Exception (486+)
_isr17:
    cli
    push 0
    push 17
	jmp isr_common_stub
	
#  18: Machine Check Exception (Pentium/586+)
_isr18:
    cli
    push 0
    push 18
	jmp isr_common_stub
	
#  19: Reserved Exception
_isr19:
    cli
    push 0
    push 19
	jmp isr_common_stub
	
#  20: Reserved Exception
_isr20:
    cli
    push 0
    push 20
	jmp isr_common_stub
	
#  21: Reserved Exception
_isr21:
    cli
    push 0
    push 21
	jmp isr_common_stub
	
#  22: Reserved Exception
_isr22:
    cli
    push 0
    push 22
	jmp isr_common_stub
	
#  23: Reserved Exception
_isr23:
    cli
    push 0
    push 23
	jmp isr_common_stub
	
#  24: Reserved Exception
_isr24:
    cli
    push 0
    push 24
	jmp isr_common_stub
	
#  25: Reserved Exception
_isr25:
    cli
    push 0
    push 25
	jmp isr_common_stub
	
#  26: Reserved Exception
_isr26:
    cli
    push 0
    push 26
	jmp isr_common_stub
	
#  27: Reserved Exception
_isr27:
    cli
    push 0
    push 27
	jmp isr_common_stub
	
#  28: Reserved Exception
_isr28:
    cli
    push 0
    push 28
	jmp isr_common_stub
	
#  29: Reserved Exception
_isr29:
    cli
    push 0
    push 29
	jmp isr_common_stub
	
#  30: Reserved Exception
_isr30:
    cli
    push 0
    push 30
	jmp isr_common_stub
	
#  31: Reserved Exception
_isr31:
    cli
    push 0
    push 31
	jmp isr_common_stub

.extern _fault_handler

isr_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    movw $0x10, %ax # Load the Kernel Data Segment descriptor!
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movl %esp, %eax # Push us the stack
    push %eax
    movl $_fault_handler, %eax
    call *%eax       # A special call, preserves the 'eip' register
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    addl $8, %esp  # Cleans up the pushed error code and pushed ISR number
    iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
    
#IRQ
.global _irq0
.global _irq1
.global _irq2
.global _irq3
.global _irq4
.global _irq5
.global _irq6
.global _irq7
.global _irq8
.global _irq9
.global _irq10
.global _irq11
.global _irq12
.global _irq13
.global _irq14
.global _irq15

# 32: IRQ0
_irq0:
    cli
    push 0    # Note that these don't push an error code on the stack:
                   # We need to push a dummy error code
    push 32
    jmp irq_common_stub

# 33: IRQ1
_irq1:
    cli
    push 0
    push 33
    jmp irq_common_stub

# 34: IRQ2
_irq2:
    cli
    push 0
    push 34
    jmp irq_common_stub

# 35: IRQ3
_irq3:
    cli
    push 0
    push 35
    jmp irq_common_stub

# 36: IRQ4
_irq4:
    cli
    push 0
    push 36
    jmp irq_common_stub

# 37: IRQ5
_irq5:
    cli
    push 0
    push 37
    jmp irq_common_stub

# 38: IRQ6
_irq6:
    cli
    push 0
    push 38
    jmp irq_common_stub

# 39: IRQ7
_irq7:
    cli
    push 0
    push 39
    jmp irq_common_stub

# 40: IRQ8
_irq8:
    cli
    push 0
    push 40
    jmp irq_common_stub

# 41: IRQ9
_irq9:
    cli
    push 0
    push 41
    jmp irq_common_stub

# 42: IRQ10
_irq10:
    cli
    push 0
    push 42
    jmp irq_common_stub

# 43: IRQ11
_irq11:
    cli
    push 0
    push 43
    jmp irq_common_stub

# 44: IRQ12
_irq12:
    cli
    push 0
    push 44
    jmp irq_common_stub

# 45: IRQ13
_irq13:
    cli
    push 0
    push 45
    jmp irq_common_stub

# 46: IRQ14
_irq14:
    cli
    push 0
    push 46
    jmp irq_common_stub

# 47: IRQ15
_irq15:
    cli
    push 0
    push 47
    jmp irq_common_stub

.extern _irq_handler

# This is a stub that we have created for IRQ based ISRs. This calls
# '_irq_handler' in our C code. We need to create this in an 'irq.c'
irq_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movl %esp, %eax
    push %eax
    movl $_irq_handler, %eax
    call *%eax
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    addl $8, %esp 
    iret  
