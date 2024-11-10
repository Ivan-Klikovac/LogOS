#include <stdio.h>
#include <gdt.h>
#include <kbd.h>

// GDT, IDT, TSS and APIC stuff

gdt_entry gdt[5]; // null, code, data, tss 32, tss 64
gdt_ptr gdtr;

idt_entry idt[256];
idt_ptr idtr; // to be loaded into IDTR

tss tr = {0};

uint8_t interrupt_stack[4096];
void* istacktop = (void*) &interrupt_stack;

volatile uint32_t* lapic_base = (uint32_t*) 0xFEE00000;
volatile uint32_t* ioapic_base = (uint32_t*) 0xFEC00000;

void ioapic_check(void);
void lapic_check(void);

void gdt_init(void)
{
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint64_t) &gdt;

    gdt_set_entry(0, 0, 0, 0, 0); // null descriptor
    gdt_set_entry(1, 0, 0, 0x9A, 0xA0); // kernel code segment
    gdt_set_entry(2, 0, 0, 0x92, 0xA0); // kernel data segment

    asm volatile("lgdt %0" : : "m"(gdtr) : "memory");

    // reload segment registers
    asm volatile("mov $0x10, %%ax\n" // 0x10 is entry 2 in the GDT
                "mov %%ax, %%ds\n" 
                "mov %%ax, %%es\n"
                "mov %%ax, %%fs\n"
                "mov %%ax, %%gs\n"
                "mov %%ax, %%ss\n"
                : : : "memory");
}

void gdt_set_entry(int n, uint32_t base, uint32_t limit, uint8_t a, uint8_t g)
{
    gdt[n].base_low = base & 0xFFFF;
    gdt[n].base_middle = (base >> 16) & 0xFF;
    gdt[n].base_high = (base >> 24) & 0xFF;
    gdt[n].limit_low = limit & 0xFFFF;
    gdt[n].granularity = (limit >> 16) & 0x0F;
    gdt[n].granularity |= (g & 0xF0);
    gdt[n].access = a;
}

void idt_init(void)
{
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint64_t) &idt;
    asm volatile("lidt %0" : : "m"(idtr) : "memory");

    lapic_init();
    ioapic_init();

    idt_set_entry(0x20, (uint64_t) isr_timer);
    idt_set_entry(0x21, (uint64_t) isr_kbd);

    asm volatile("sti" : : : "memory");

    /*while (1) {
        if (inb(0x64) & 0x1) { // Status register, bit 0: Output buffer status
            uint8_t scancode = inb(0x60);
            printf("scancode: %x\n", scancode);  // Print the scancode
        }
    }*/

    //ioapic_check();
    lapic_check();
}

void idt_set_entry(int n, uint64_t handler)
{
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].ist = 0;
    idt[n].type_attr = 0x8E;
    idt[n].offset_mid = (handler >> 16) & 0xFFFF;
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].zero = 0;
}

void tss_init(void)
{
    tr.rsp0 = (uint64_t) istacktop;
    uint64_t base = (uint64_t) &tr;
    uint32_t limit = sizeof(tr) - 1;

    // set up the 32-bit TSS
    // could do this as a gdt_set_entry() call
    gdt[3].limit_low = limit & 0xFFFF;
    gdt[3].base_low = base & 0xFFFF;
    gdt[3].base_middle = (base >> 16) & 0xFF;
    gdt[3].access = 0x89;
    gdt[3].granularity = (limit >> 16) & 0x0F;
    gdt[3].base_high = (base >> 24) & 0xFF;

    // next entry for 64-bit TSS
    gdt[4].base_high = (base >> 32) & 0xFFFFFFFF;

    asm volatile("ltr %0" : : "r"((uint16_t) 0x18) : "memory");
}

// IRQs 0-15 are mapped to interrupts 0x08-0x0F
// which conflicts with CPU exceptions
// they first need to be remapped
void pic_remap(void)
{
    // initialize master and slave PIC (cascade mode)
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // remap master to 0x20 and slave to 0x28
    outb(0x21, 0x20); // master vector offset
    outb(0xA1, 0x28); // slave vector offset

    outb(0x21, 0x04); // tell master about slave at IRQ2
    outb(0xA1, 0x02); // tell slave it is cascade

    // mask all PIC interrupts, we'll use APIC
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

static inline uint32_t lapic_read(uint32_t reg)
{
    return lapic_base[reg / 4];
}

static inline void lapic_write(uint32_t reg, uint32_t value)
{
    lapic_base[reg / 4] = value;
}


void lapic_init(void)
{
    printf("setting up LAPIC...\n");
    // enable LAPIC by setting bit 8 in the spurious interrupt vector reg
    //lapic_write(0xF0, lapic_read(0xF0) | 0x100);

    // set the spurious interrupt vector to a non-reserved value
    lapic_write(0xF0, 0x100 | SPURIOUS_INTERRUPT_VECTOR);

    // acknowledge any outstanding interrupts
    lapic_write(0xB0, 0);
}

static inline uint32_t ioapic_read(uint8_t reg)
{
    ioapic_base[0] = reg;
    return ioapic_base[4];
}

static inline void ioapic_write(uint8_t reg, uint32_t value)
{
    ioapic_base[0] = reg;
    ioapic_base[4] = value;
}

void ioapic_init(void)
{
    uint8_t irq = 1; // keyboard
    uint32_t vector = 0x21; // mapping it to 0x21
    printf("setting up IOAPIC for IRQ %d to vector 0x%x...\n", irq, vector);
    ioapic_write(0x10 + 2 * irq, vector); // lower 32b, interrupt vector
    ioapic_write(0x10 + 2 * irq + 1, 0); // upper 32b, destination field
}

// =============================================================================
// ============ I N T E R R U P T   S E R V I C E   R O U T I N E S ============
// =============================================================================

// each interrupt receives a stack frame

__attribute__((interrupt)) void isr_timer(void* frame)
{
    putchar('t');
}

__attribute__((interrupt)) void isr_kbd(void* frame)
{
    // read the scancode from 0x60 (kbd I/O port)
    uint8_t scancode = inb(0x60);

    // handle 
    kbdint(scancode);

    // acknowledge
    lapic_write(0xB0, 0);
}

void ioapic_check(void)
{
    uint32_t ioapic_redir = ioapic_read(0x10 + 2 * 1); // IRQ 1, lower 32 bits
    uint32_t ioapic_dest = ioapic_read(0x10 + 2 * 1 + 1); // IRQ 1, upper 32 bits
    printf("IOAPIC redirection table for IRQ 1: 0x%x%x\n", ioapic_dest, ioapic_redir);
}

void lapic_check(void)
{
    uint32_t res = lapic_read(0xF0);
    printf("spurious interrupt vector register: 0x%x (%sOK)\n", res, res & 0x100 ? "" : "not ");
    uint32_t lapic_tpr = lapic_read(0x80);
    printf("task priority register: 0x%x (%sOK)\n", lapic_tpr, lapic_tpr ? "not " : "");
}
