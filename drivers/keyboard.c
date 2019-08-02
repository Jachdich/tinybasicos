#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "videodriver.h"
#include "../libc/string.h"
#include "../libc/data.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SHIFT 0x2A
#define LSHIFT 0x36
#define ALT 0x37
#define TAB 0xf
#define CTRL 0x1d

static char key_buffer[256];
int keysPressed[60];

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = { 'a', 'b', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', 'c', 0x0, 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', 'e', 0x0, 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', 0x0, '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', 0x0, 'g', 0x0, ' '};

int shift() {
    return keysPressed[SHIFT] + keysPressed[LSHIFT] > 0;
}

int ctrl() {
    return keysPressed[CTRL];
}

static void keyboard_callback(registers_t regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    u8 scancode = port_byte_in(0x60);
    
    if (scancode > SC_MAX) {
        keysPressed[(int)scancode - 128] = 0;
        return;
    }
    keysPressed[(int)scancode] = 1;
    if (scancode == BACKSPACE) {
        backspace(key_buffer);
        print_backspace();
    } else if (scancode == ENTER) {
        print("\n");
        user_input(key_buffer); /* kernel-controlled function */
        key_buffer[0] = '\0';
    } else {
        char letter = sc_ascii[(int)scancode];
        if (!shift()) {
            letter = toLowerCase(letter);
        }

        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        print(str);
    }
    UNUSED(regs);
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}