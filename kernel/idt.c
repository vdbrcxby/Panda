
// @Name   : idt.c 
//
// @Author : Yukang Chen (moorekang@gmail.com)
// @Date   : 2012-01-03 22:48:21
//
// @Brief  : Interrupt Gate Descriptor related

#include <system.h>
#include <screen.h>
#include <asm.h>

/*
Interrupt Gate Descriptor

63                       48|47           40|39               32  
+------------------------------------------------------------
|                         | |D|D| | | | | | | | |           
|   BASE OFFSET (16-31)   |P|P|P|0|1|1|1|0|0|0|0| RESERVED  
|                         | |L|L| | | | | | | | |           
=============================================================
                           |                                |
   SEGMENT SELECTOR        |   BASE  OFFSET (0-15)          |
                           |                                |
------------------------------------------------------------+
31                       16|15                               0

         - bits   0 to 15 : base offset low
         - bits 16 to 31  : segment selector
         - bits 32 to 37  : reserved
         - bits 37 to 39  : 0
         - bits 40 to 47  : flags/type 
         - bits 48 to 63  : base offset high

*/    


/* Defines an IDT entry */
struct idt_entry
{
    unsigned short base_lo;
    unsigned short selector;
    unsigned char  zero;
    unsigned char  flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

/* The IDT table , used for interrupt */
struct idt_entry idt[256];
struct idt_ptr   idtp;

/* This exists in 'start.asm', and is used to load our IDT */
extern void idt_load();

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();



const char* exception_messages[32] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* similar with GDT */
void idt_set(unsigned char k, unsigned long base,
             unsigned short selector, unsigned char flags)
{
    /* The interrupt routine's base address */
    idt[k].base_lo = (base & 0xFFFF);
    idt[k].base_hi = (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[k].selector = selector;
    idt[k].zero = 0;
    idt[k].flags = flags;
}

extern isq_t irq_routines[];

/* Installs the IDT */
void idt_init()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (u32)&idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);


    // Remap the irq table.
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


    /* Add any new ISRs to the IDT here using idt_set_gate */
    /* Points the processor's internal register to the new IDT */

    idt_set( 0, (u32)isr0 , 0x08, 0x8E);
    idt_set( 1, (u32)isr1 , 0x08, 0x8E);
    idt_set( 2, (u32)isr2 , 0x08, 0x8E);
    idt_set( 3, (u32)isr3 , 0x08, 0x8E);
    idt_set( 4, (u32)isr4 , 0x08, 0x8E);
    idt_set( 5, (u32)isr5 , 0x08, 0x8E);
    idt_set( 6, (u32)isr6 , 0x08, 0x8E);
    idt_set( 7, (u32)isr7 , 0x08, 0x8E);
    idt_set( 8, (u32)isr8 , 0x08, 0x8E);
    idt_set( 9, (u32)isr9 , 0x08, 0x8E);
    idt_set(10, (u32)isr10, 0x08, 0x8E);
    idt_set(11, (u32)isr11, 0x08, 0x8E);
    idt_set(12, (u32)isr12, 0x08, 0x8E);
    idt_set(13, (u32)isr13, 0x08, 0x8E);
    idt_set(14, (u32)isr14, 0x08, 0x8E);
    idt_set(15, (u32)isr15, 0x08, 0x8E);
    idt_set(16, (u32)isr16, 0x08, 0x8E);
    idt_set(17, (u32)isr17, 0x08, 0x8E);
    idt_set(18, (u32)isr18, 0x08, 0x8E);
    idt_set(19, (u32)isr19, 0x08, 0x8E);
    idt_set(20, (u32)isr20, 0x08, 0x8E);
    idt_set(21, (u32)isr21, 0x08, 0x8E);
    idt_set(22, (u32)isr22, 0x08, 0x8E);
    idt_set(23, (u32)isr23, 0x08, 0x8E);
    idt_set(24, (u32)isr24, 0x08, 0x8E);
    idt_set(25, (u32)isr25, 0x08, 0x8E);
    idt_set(26, (u32)isr26, 0x08, 0x8E);
    idt_set(27, (u32)isr27, 0x08, 0x8E);
    idt_set(28, (u32)isr28, 0x08, 0x8E);
    idt_set(29, (u32)isr29, 0x08, 0x8E);
    idt_set(30, (u32)isr30, 0x08, 0x8E);
    idt_set(31, (u32)isr31, 0x08, 0x8E);
    
    idt_set(32, (u32)irq0, 0x08, 0x8E);
    idt_set(33, (u32)irq1, 0x08, 0x8E);
    idt_set(34, (u32)irq2, 0x08, 0x8E);
    idt_set(35, (u32)irq3, 0x08, 0x8E);
    idt_set(36, (u32)irq4, 0x08, 0x8E);
    idt_set(37, (u32)irq5, 0x08, 0x8E);
    idt_set(38, (u32)irq6, 0x08, 0x8E);
    idt_set(39, (u32)irq7, 0x08, 0x8E);

    idt_set(40, (u32)irq8, 0x08, 0x8E);
    idt_set(41, (u32)irq9, 0x08, 0x8E);
    idt_set(42, (u32)irq10, 0x08, 0x8E);
    idt_set(43, (u32)irq11, 0x08, 0x8E);
    idt_set(44, (u32)irq12, 0x08, 0x8E);
    idt_set(45, (u32)irq13, 0x08, 0x8E);
    idt_set(46, (u32)irq14, 0x08, 0x8E);
    idt_set(47, (u32)irq15, 0x08, 0x8E);

    /* just flush this */
    asm volatile(
        "lidt %0"
        :: "m"(idtp));

    memset(&irq_routines , 0, sizeof(isq_t)*256);
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(struct registers_t* regs)
{
    puts("recieved interrupt: ");
    printk_hex(regs->int_no);
    puts("\n");

     /* Is this a fault whose number is from 0 to 31? */
    if (regs->int_no < 32)
    {
        puts(exception_messages[regs->int_no]);
        puts("\n");
        //        for (;;);
    }
}
