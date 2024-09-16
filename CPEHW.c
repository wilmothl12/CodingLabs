// Logan Wilmoth and Jack Covino

#include "address_map_arm.h"
#include "interrupt_ID.h"
#include "defines.h"
#include "stdio.h"

void disable_A9_interrupts(void);
void set_A9_IRQ_stack(void);
void config_interrupt(int, int);
void config_GIC(void);
void config_TIMER(void);
void config_KEYs(void);
void enable_A9_interrupts(void);
void timer_ISR(void);
void keyboard_ISR(void);
int hexToDecimal(int);

volatile int button_count = 0;
volatile int timer_delay = 4;
volatile int display_in_hex = 1;
volatile int *LEDR_ptr = (int *)LEDR_BASE;

int main(void) {
    disable_A9_interrupts();
    set_A9_IRQ_stack();
    config_interrupt(KEYS_IRQ, CPU0);
    config_interrupt(HPS_TIMER0_IRQ, CPU0);
    config_GIC();
    config_TIMER();
    config_KEYs();
    enable_A9_interrupts();

    while (1) {
    }
}

// Common Interrupt Service Routine for Timer and Keyboard
void __cs3_isr_irq(void) {
    // Determine the interrupt source
    int int_ID = *((int *)INTERRUPT_SOURCE);
    
    // Timer 0 interrupt
    if (int_ID == HPS_TIMER0_IRQ) {
        timer_ISR();
    }
    // Keyboard interrupt
    else if (int_ID == KEYS_IRQ) {
        keyboard_ISR();
    }

    // Acknowledge the interrupt
    *((int *)0xFFFEC10C) = int_ID;
}

// Timer 0 Interrupt Service Routine
void timer_ISR(void) {
    volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
    volatile int *TIMER0_ptr = (int *)TIMER0_BASE;

    // Clear all LEDs
    *LEDR_ptr = 0;

    // Display the number of buttons pressed on 7-segment display
    if (display_in_hex) {
        *HEX3_HEX0_ptr = button_count;
    } else {
        // Convert button_count to decimal and display
        int decimal_value = hexToDecimal(button_count);
        *HEX3_HEX0_ptr = decimal_value;
    }

    // Adjust timer delay between 4 and 10 seconds
    if (timer_delay > 4) {
        timer_delay--;
    }

    *TIMER0_ptr = timer_delay;  // Set the new timer delay

    // Clear the timer interrupt
    *(TIMER0_ptr + 3) = 1;
}

// Keyboard Interrupt Service Routine
void keyboard_ISR(void) {
    volatile int *KEY_ptr = (int *)KEY_BASE;

    int key_pressed = *(KEY_ptr + 3);

    switch (key_pressed) {
        case 0x1:
            *LEDR_ptr = 0x1;  // Turn on LEDR1 for Key0
            break;
        case 0x2:
            *LEDR_ptr = 0x2;  // Turn on LEDR2 for Key1
            // Increase the time delay by 1 second, but not beyond 10 seconds
            if (timer_delay < 10) {
                timer_delay++;
            }
            break;
        case 0x4:
            *LEDR_ptr = 0x4;  // Turn on LEDR3 for Key2
            // Decrease the time delay by 1 second, but not below 4 seconds
            if (timer_delay > 4) {
                timer_delay--;
            }
            break;
        case 0x8:
            *LEDR_ptr = 0x8;  // Turn on LEDR4 for Key3
            // Toggle the format for displaying the number of button presses
            display_in_hex = !display_in_hex;
            break;
        default:
            break;
    }

    // Clear the keyboard interrupt
    *(KEY_ptr + 3) = 0xF;
}

// Function to convert hexadecimal to decimal
int hexToDecimal(int hexValue) {
    int decimalValue = 0;
    int base = 1;

    while (hexValue > 0) {
        int remainder = hexValue % 16;
        decimalValue = decimalValue + remainder * base;
        hexValue = hexValue / 16;
        base = base * 10;
    }

    return decimalValue;
}

// Function to configure Timer 0
void config_TIMER(void) {
    volatile int *TIMER0_ptr = (int *)TIMER0_BASE;

    // Disable Timer
    *(TIMER0_ptr + 2) = 0;  // Control Register: Stop Timer

    // Set the timer period
    int timer_period = 50000000;  // 50 million cycles for 1 second
    *TIMER0_ptr = timer_period;

    // Configure the Control Register to generate interrupts and enable the timer
    *(TIMER0_ptr + 2) = 0b011;  // Enable Timer, Enable Load Register, Interrupt Enable

    // Clear existing interrupt
    *(TIMER0_ptr + 3) = 1;
}

// Function to configure Keyboard interrupts
void config_KEYs(void) {
    volatile int *KEY_ptr = (int *)KEY_BASE;

    // Enable interrupts for all four keys
    *(KEY_ptr + 2) = 0xF;
}