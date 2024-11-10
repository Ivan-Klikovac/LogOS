#include <bootboot.h>
#include <vm.h>

char free[] = "FREE";
char acpi[] = "ACPI";
char mmio[] = "MMIO";
char used[] = "USED";

char* code_to_str(int code)
{
    if(code == MMAP_FREE) return free;
    if(code == MMAP_ACPI) return acpi;
    if(code == MMAP_MMIO) return mmio;
    if(code == MMAP_USED) return used;
    return used; 
}

void vm_test(void)
{
    printf("mmap entries: %u\n", (bootboot.size - 128)/16);
    MMapEnt* mmap_entry = &bootboot.mmap;
    int i = 0;
    while(mmap_entry->ptr || mmap_entry->size) {
        printf("%d) %x-%x (size: %d KB) [%s]\n", i, mmap_entry->ptr,
        mmap_entry->ptr + MMapEnt_Size(mmap_entry),
        mmap_entry->size / 1024, code_to_str(MMapEnt_Type(mmap_entry)));
        
        mmap_entry++;
        i++;
    }

    // CR3 is 00000000_0000a000
    //printf("testing 15000-9FC00");
    //for(char* i = (char*) 0x15000; i < (char*) 0x9FC00; i++) *i = 0;
    //printf("one\n");
    //for(char* i = (char*) 0xAFF9; i < (char*) 0xafff; i++) *i = 0;
}