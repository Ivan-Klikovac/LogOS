#include <stdio.h>
#include <kbd.h>
#include <console.h>

const char keymap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

const char shift_keymap[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

int shift = 0;
int ctrl = 0;
int caps = 0;

void kbdint(uint8_t scancode) {
    if(scancode & 0x80) { // key release (bit 7 is set)
        scancode &= 0x7F; // mask out the key release bit
        if(scancode == LSHIFT || scancode == RSHIFT) shift = 0;
        else if(scancode == LCTRL || scancode == RCTRL) ctrl = 0;
    }

    else { // key press
        if(scancode == LSHIFT || scancode == RSHIFT) shift = 1;
        else if(scancode == LCTRL || scancode == RCTRL) ctrl = 1;
        else if(scancode == CAPS) caps = !caps;
        
        else {
            char c = 0;
            if(shift || (caps && c >= 'a' && c <= 'z'))
                c = shift_keymap[scancode];
            else c = keymap[scancode];
            consoleint(c);
        }
    }
}