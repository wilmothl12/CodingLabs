// Logan Wilmoth
// Lab 5 Part 1

#include "address_map_arm.h"
#include "interrupt_ID.h"
#include "defines.h"

void disable_A9_interrupts(void);
void set_A9_IRQ_stack(void);
void config_interrupt(int, int);
void config_GIC(void);
void config_KEYs(void);
void config_HPS_timer(void);
void enable_A9_interrupts(void);

volatile int *ledreg = (volatile int *)0xFF200000;
int direction = 0, run = 1, KEY_num, led_bits = 0x0, reset = 0, counter = 0, blink = 1;

/* ********************************************************************************
 * This program demonstrates use of interrupts with C code.  The program responds
 * to interrupts from the pushbutton KEY port in the FPGA.
 *
 * The interrupt service routine for the KEYs indicates which KEY has been pressed
 * on the display HEX0.
 ********************************************************************************/
int main(void)
{
	disable_A9_interrupts();		  // disable interrupts in the A9 processor
	set_A9_IRQ_stack();				  // initialize the stack pointer for IRQ mode
	config_interrupt(KEYS_IRQ, CPU0); // configure the FPGA KEYs interrupt
	config_interrupt(199, CPU0);
	config_GIC();  // configure the general interrupt controller
	config_KEYs(); // configure pushbutton KEYs to generate interrupts
	config_HPS_timer();
	enable_A9_interrupts(); // enable interrupts in the A9 processor
	*ledreg = led_bits;

	while (1)
	{
		if (blink == 1)
		{
			*ledreg = counter;
		}
		else
		{
			*ledreg = 0;
		}
	}
}
void config_HPS_timer()
{
	volatile int *HPS_timer_ptr = (int *)HPS_TIMER0_BASE;
	*(HPS_timer_ptr + 0x2) = 0;
	int counter = 25000000;
	*(HPS_timer_ptr) = counter;
	*(HPS_timer_ptr + 2) = 0b011;
}
/* setup the KEY interrupts in the FPGA */
void config_KEYs()
{
	volatile int *KEY_ptr = (int *)KEY_BASE; // pushbutton KEY base address
	*(KEY_ptr + 2) = 0xF;					 // enables interrupts for all KEYS
}

/****************************************************************************************
 * Pushbutton - Interrupt Service Routine
 *
 * This routine checks which KEY has been pressed. It writes to HEX0
 ***************************************************************************************/
void HPS_timer_ISR()
{
	volatile int *HPS_timer_ptr = (int *)HPS_TIMER0_BASE;
	*(HPS_timer_ptr + 3);

	if (reset == 1)
	{
		if (direction == 0)
		{
			counter = 0;
		}
		else
		{
			counter = 0x3FF;
		}
		reset = 0;
	}
	if (run == 1)
	{
		if (direction == 0)
		{
			counter++;
			if (counter > 0x3FF)
			{
				counter = 0;
			}
		}
		else
		{
			counter--;
			if (counter < 0)
			{
				counter = 0x3FF;
			}
		}
	}
	if (run == 0)
	{
	}
}
void pushbutton_ISR(void)
{
	volatile int *KEY_ptr = (int *)KEY_BASE;
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	int press, HEX_bits;
	press = *(KEY_ptr + 3); // reads the pushbutton interrupt
	*(KEY_ptr + 3) = press; // clears the interrupt
	if (press & 0x1)
	{ // KEY0
		HEX_bits = 0b00111111;
		KEY_num = 1;
		if (KEY_num == 1)
		{
			if (direction == 0)
				led_bits = 0x0;
			else
				led_bits == 0x3ff;
			reset = 1;
			KEY_num = 0;
		}
	}
	else if (press & 0x2)
	{ // KEY1
		HEX_bits = 0b00000110;
		KEY_num = 2;
		if (KEY_num == 2)
		{
			if (run == 0)
				run = 1;
			else
				run = 0;
			KEY_num = 0;
		}
	}
	else if (press & 0x4)
	{ // KEY2
		HEX_bits = 0b01011011;
		KEY_num = 3;
		if (KEY_num = 3)
		{
			if (direction == 0)
				direction = 1;
			else
				direction = 0;
			KEY_num = 0;
		}
	}
	else
	{ // KEY3
		HEX_bits = 0b01001111;
		KEY_num = 4;
		if (KEY_num = 4)
		{
			*ledreg = 0x0;
			KEY_num = 0;
		}
		blink ^= 1;
	}
	*HEX3_HEX0_ptr = HEX_bits;
	return;
}
