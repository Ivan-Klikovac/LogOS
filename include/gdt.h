#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

#define SPURIOUS_INTERRUPT_VECTOR 0xEF

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) gdt_ptr;

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed)) idt_entry;

typedef struct {
    uint16_t limit; // size
    uint64_t base; // base address
} __attribute__((packed)) idt_ptr;

typedef struct {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iomap_base;
} __attribute__((packed)) tss;

static inline void lidt(uint64_t idt_ptr)
{
    asm volatile("lidt (%0)" : : "r" (idt_ptr) : "memory");
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port) : "memory");
    return value;
}

static inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

void gdt_init(void);
void gdt_set_entry(int, uint32_t, uint32_t, uint8_t, uint8_t);

void idt_init(void);
void idt_set_entry(int, uint64_t);

void tss_init(void);

void lapic_init(void);
void ioapic_init(void);

__attribute__((interrupt)) void isr_timer(void* frame);
__attribute__((interrupt)) void isr_kbd(void* frame);

#endif