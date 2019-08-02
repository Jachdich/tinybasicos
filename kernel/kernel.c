#include "../drivers/videodriver.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../cpu/timer.h"
#include "../libc/string.h"

int kernel_main() {
    clearScreen();
    //if we get this far, we know the following messages would
    //have been printed on the screen by the BIOS/boot sector
    //so replicate them as a log
    println("OS starting...");
    println("Loading kernel...");
    println("Starting kernel...");
    println("Kernel successfully started");
    println("Doing some magic with the ISRs...");
    isr_install();
    println("Magic with the ISRs has been done");
    
    print("Enabling interrupts... ");
    asm volatile("sti");
    println("Done.");
    print("Initialising timer... ");
    init_timer(50);
    println("Done.");
    /* Comment out the timer IRQ handler to read
     * the keyboard IRQs easier */
    print("Initialising keyboard... ");
    init_keyboard();
    println("Done.");
    print("12345");
    print_backspace();
    while (1);
}

void user_input(char * in) {
    if (strcmp(in, "end") == 1) {
        println("End it shall be!");
    }
}